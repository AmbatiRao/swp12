package swp12.apollonius.evaluation;

import java.io.BufferedInputStream;
import java.io.BufferedWriter;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStreamWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.Set;

import javax.xml.parsers.ParserConfigurationException;

import org.xml.sax.SAXException;

import com.vividsolutions.jts.geom.Coordinate;
import com.vividsolutions.jts.geom.Envelope;
import com.vividsolutions.jts.geom.Geometry;
import com.vividsolutions.jts.geom.GeometryFactory;
import com.vividsolutions.jts.geom.Point;
import com.vividsolutions.jts.io.ParseException;
import com.vividsolutions.jts.io.WKTReader;

import de.topobyte.carbon.geometry.speedup.containment.ContainmentTester;
import de.topobyte.carbon.geometry.speedup.containment.DiscretizingContainmentTester;
import de.topobyte.carbon.geometry.speedup.containment.DiscretizingContainmentTesterFactory;
import de.topobyte.carbon.geometry.speedup.index.ContainmentGeometryTesselationMap;
import de.topobyte.selenium.fileformat.EntityFile;
import de.topobyte.selenium.fileformat.FileReader;

public class Evaluator {

	public static void main(String[] args) throws FileNotFoundException {
		if (args.length != 5) {
			System.out
					.println("usage: "
							+ Evaluator.class.getSimpleName()
							+ " <dir with original polygons> <dir with apollonius polygons>"
							+ " <number of points to generate / point file>"
							+ " <detection rate output append file>"
							+ " <name of this sample to append to output>");
			System.exit(1);
		}

		boolean onlyCities = false;

		String pathDirSource = args[0];
		String pathDirApollonius = args[1];
		String vNum = args[2];
		String pathOutput = args[3];
		String nameSample = args[4];

		boolean randInput = true;
		File filePoints = null;

		/*
		 * parse number of points argument
		 */
		int numPoints = 0;
		try {
			numPoints = Integer.parseInt(vNum);
		} catch (NumberFormatException e) {
			System.out.println("unable to parse number of points: "
					+ e.getMessage());
			randInput = false;
			filePoints = new File(vNum);
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
				if (onlyCities) {
					int population = Integer.parseInt(vPopulation);
					if (population < 100000) {
						ids.remove(ids.size() - 1);
					}
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

		if (randInput) {
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

				ContainmentTester ctSource = new DiscretizingContainmentTester(
						sourceGeometry, 10);
				ContainmentTester ctApollonius = new DiscretizingContainmentTester(
						apolloniusGeometry, 10);

				Envelope bbox = sourceGeometry.getEnvelopeInternal();
				for (int i = 0; i < nPoints; i++) {
					Point point = generatePointWithin(ctSource, bbox, random,
							geometryFactory);
					boolean inApollonius = ctApollonius.covers(point);
					if (inApollonius) {
						nCorrect += 1;
					} else {
						nWrong += 1;
					}
				}
			}
		} else {
			DiscretizingContainmentTesterFactory containmentTesterFactory = new DiscretizingContainmentTesterFactory(
					10);
			ContainmentGeometryTesselationMap<String> index = new ContainmentGeometryTesselationMap<String>(
					containmentTesterFactory);
			for (String id : ids) {
				Geometry sourceGeometry = idToSourceGeometry.get(id);
				index.add(sourceGeometry, id);
			}

			GeometryFactory geometryFactory = new GeometryFactory();

			FileInputStream fis = new FileInputStream(filePoints);
			BufferedInputStream is = new BufferedInputStream(fis);

			int i = 0;

			while (true) {
				if ((++i % 1000) == 0) {
					System.out.println("processed: " + i);
				}
				try {
					double lon = readDouble(is);
					double lat = readDouble(is);

					Point point = geometryFactory.createPoint(new Coordinate(
							lon, lat));

					Set<String> sourceIds = index.test(point);
					if (sourceIds.size() == 0 || sourceIds.size() > 1) {
						continue;
					}
					String id = sourceIds.iterator().next();
					Geometry apolloniusGeometry = idToApolloniusGeometry
							.get(id);

					if (apolloniusGeometry == null) {
						nWrong += 1;
						continue;
					}

					boolean inApollonius = apolloniusGeometry.covers(point);
					if (inApollonius) {
						nCorrect += 1;
					} else {
						nWrong += 1;
					}
				} catch (IOException e) {
					break;
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
		System.out.println(String.format("how many points are correct? %.3f",
				correct));
		System.out.println(String.format("how many points are wrong?   %.3f",
				wrong));

		File fileOutput = new File(pathOutput);
		FileOutputStream output = new FileOutputStream(fileOutput, true);
		BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(
				output));
		try {
			writer.write(String.format("%s: %f", nameSample, correct));
			writer.write(System.getProperty("line.separator"));
			writer.close();
		} catch (IOException e) {
			System.out.println("error while appending to output: "
					+ e.getMessage());
		}
	}

	private static double readDouble(BufferedInputStream is) throws IOException {
		long bits = 0;
		bits |= ((long) readByte(is)) << 56;
		bits |= ((long) readByte(is)) << 48;
		bits |= ((long) readByte(is)) << 40;
		bits |= ((long) readByte(is)) << 32;
		bits |= ((long) readByte(is)) << 24;
		bits |= readByte(is) << 16;
		bits |= readByte(is) << 8;
		bits |= readByte(is);
		return Double.longBitsToDouble(bits);
	}

	private static int readByte(InputStream is) throws IOException {
		int read = is.read();
		if (read < 0) {
			throw new IOException("end of stream");
		}
		return read;
	}

	private static Point generatePointWithin(ContainmentTester tester,
			Envelope bbox, Random random, GeometryFactory geometryFactory) {
		while (true) {
			double minX = bbox.getMinX();
			double maxX = bbox.getMaxX();
			double minY = bbox.getMinY();
			double maxY = bbox.getMaxY();
			double x = minX + ((maxX - minX) * random.nextDouble());
			double y = minY + ((maxY - minY) * random.nextDouble());
			Point point = geometryFactory.createPoint(new Coordinate(x, y));
			if (tester.covers(point)) {
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
