package swp12.apollonius.evaluation;

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;

import javax.xml.parsers.ParserConfigurationException;

import org.xml.sax.SAXException;

import com.vividsolutions.jts.geom.Coordinate;
import com.vividsolutions.jts.geom.Envelope;
import com.vividsolutions.jts.geom.Geometry;
import com.vividsolutions.jts.geom.GeometryFactory;
import com.vividsolutions.jts.geom.Point;
import com.vividsolutions.jts.io.ParseException;
import com.vividsolutions.jts.io.WKTReader;

import de.topobyte.selenium.fileformat.EntityFile;
import de.topobyte.selenium.fileformat.FileReader;

public class Evaluator {

	public static void main(String[] args) {
		if (args.length != 3) {
			System.out
					.println("usage: "
							+ Evaluator.class.getSimpleName()
							+ " <dir with original polygons> <dir with apollonius polygons> <number of points to generate>");
			System.exit(1);
		}

		String pathDirSource = args[0];
		String pathDirApollonius = args[1];
		String vNum = args[2];

		/*
		 * parse number of points argument
		 */
		int numPoints = 0;
		try {
			numPoints = Integer.parseInt(vNum);
		} catch (NumberFormatException e) {
			System.out.println("unable to parse number of points: "
					+ e.getMessage());
		}

		/*
		 * paths
		 */
		File dirSource = new File(pathDirSource);
		File dirApollonius = new File(pathDirApollonius);

		/*
		 * Read geometries and store them for later usage
		 */
		Map<String, Geometry> idToSourceGeometry = new HashMap<String, Geometry>();
		Map<String, Geometry> idToApolloniusGeometry = new HashMap<String, Geometry>();
		List<String> ids = new ArrayList<String>();

		/*
		 * Read source geometries
		 */
		System.out.println("Reading source geometries");
		File[] filesSource = dirSource.listFiles();
		Arrays.sort(filesSource);

		for (File file : filesSource) {
			String name = file.getName();
			String id = name.replace(".smx", "");
			ids.add(id);

			try {
				EntityFile entity = FileReader.read(file);
				Geometry geometry = entity.getGeometry();
				Map<String, String> tags = entity.getTags();
				String vPopulation = tags.get("population");
				int population = Integer.parseInt(vPopulation);
				if (population < 100000) {
					ids.remove(ids.size() - 1);
				}
				idToSourceGeometry.put(id, geometry);
			} catch (ParserConfigurationException e) {
				System.out.println("error while reading file: "
						+ e.getMessage());
				System.exit(1);
			} catch (SAXException e) {
				System.out.println("error while reading file: "
						+ e.getMessage());
				System.exit(1);
			} catch (IOException e) {
				System.out.println("error while reading file: "
						+ e.getMessage());
				System.exit(1);
			}
		}

		/*
		 * Read apollonius geometries
		 */
		System.out.println("Reading apollonius geometries");
		File[] filesApollonius = dirApollonius.listFiles();
		Arrays.sort(filesApollonius);

		for (File file : filesApollonius) {
			String name = file.getName();
			String id = name.replace(".wkt", "");
			try {
				Geometry geometry = readWkt(file);
				idToApolloniusGeometry.put(id, geometry);
			} catch (IOException e) {
				System.out
						.println("unable to read geometry: " + e.getMessage());
				System.exit(1);
			} catch (ParseException e) {
				System.out
						.println("unable to read geometry: " + e.getMessage());
				System.exit(1);
			}

		}

		/*
		 * calculate area for each source polygon and store it in a map for
		 * later retrieval. while at hand, also calculate the total area.
		 */
		System.out.println("calculating areas");
		Map<String, Double> areas = new HashMap<String, Double>();
		double totalArea = 0;

		for (String id : ids) {
			Geometry geometry = idToSourceGeometry.get(id);
			double area = geometry.getArea();
			areas.put(id, area);
			totalArea += area;
		}

		System.out.println("total area: " + totalArea);

		int nCorrect = 0;
		int nWrong = 0;

		/*
		 * for each source geometry, calculate some test points
		 */
		Random random = new Random();
		GeometryFactory geometryFactory = new GeometryFactory();
		for (String id : ids) {
			Geometry sourceGeometry = idToSourceGeometry.get(id);
			Geometry apolloniusGeometry = idToApolloniusGeometry.get(id);
			double area = areas.get(id);
			int nPoints = (int) Math.ceil((area / totalArea) * numPoints);

			if (apolloniusGeometry == null) {
				nWrong += nPoints;
				continue;
			}

			Envelope bbox = sourceGeometry.getEnvelopeInternal();
			for (int i = 0; i < nPoints; i++) {
				Point point = generatePointWithin(sourceGeometry, bbox, random,
						geometryFactory);
				boolean inApollonius = apolloniusGeometry.contains(point);
				if (inApollonius) {
					nCorrect += 1;
				} else {
					nWrong += 1;
				}
			}
		}

		/*
		 * Print information
		 */
		int nTotal = nCorrect + nWrong;
		System.out.println("number of points generated: " + nTotal);
		System.out.println("number of correct points:   " + nCorrect);
		System.out.println("number of wrong points:     " + nWrong);
		double correct = nCorrect / (double) nTotal;
		double wrong = nWrong / (double) nTotal;
		System.out.println(String.format("how any points are correct? %.3f",
				correct));
		System.out.println(String.format("how any points are wrong?   %.3f",
				wrong));
	}

	private static Point generatePointWithin(Geometry geometry, Envelope bbox,
			Random random, GeometryFactory geometryFactory) {
		while (true) {
			double minX = bbox.getMinX();
			double maxX = bbox.getMaxX();
			double minY = bbox.getMinY();
			double maxY = bbox.getMaxY();
			double x = minX + ((maxX - minX) * random.nextDouble());
			double y = minY + ((maxY - minY) * random.nextDouble());
			Point point = geometryFactory.createPoint(new Coordinate(x, y));
			if (geometry.contains(point)) {
				return point;
			}
		}
	}

	/*
	 * Read a Geometry from a WKT file
	 */
	private static Geometry readWkt(File file) throws IOException,
			ParseException {
		FileInputStream fis = new FileInputStream(file);
		InputStream is = new BufferedInputStream(fis);
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		while (true) {
			byte[] buffer = new byte[1024];
			int read = is.read(buffer);
			if (read < 0) {
				break;
			}
			baos.write(buffer, 0, read);
		}
		fis.close();
		byte[] bytes = baos.toByteArray();
		String wkt = new String(bytes);
		return new WKTReader().read(wkt);
	}

}
