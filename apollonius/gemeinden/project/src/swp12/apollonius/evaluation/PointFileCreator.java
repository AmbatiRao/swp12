package swp12.apollonius.evaluation;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Arrays;
import java.util.Map;

import javax.xml.parsers.ParserConfigurationException;

import org.xml.sax.SAXException;

import com.vividsolutions.jts.geom.Geometry;
import com.vividsolutions.jts.geom.Point;

import de.topobyte.selenium.fileformat.EntityFile;
import de.topobyte.selenium.fileformat.FileReader;

public class PointFileCreator {

	public static void main(String[] args) {
		if (args.length != 2) {
			System.out.println("usage: "
					+ PointFileCreator.class.getSimpleName()
					+ " <dir with smx files> <output file>");
			System.exit(1);
		}

		String pathDir = args[0];
		String pathOutput = args[1];

		File dir = new File(pathDir);
		File[] files = dir.listFiles();

		Arrays.sort(files);

		BufferedWriter writer = null;
		try {
			writer = new BufferedWriter(new FileWriter(pathOutput));
		} catch (IOException e) {
			System.out.println("unable to open output file: " + e.getMessage());
			System.exit(1);
		}

		for (File file : files) {
			String name = file.getName();
			String id = name.replace(".smx", "");
			System.out.println(id);

			try {
				EntityFile entity = FileReader.read(file);

				Geometry geometry = entity.getGeometry();
				Point centroid = geometry.getCentroid();

				Map<String, String> tags = entity.getTags();
				String populationV = tags.get("population");
				int population = Integer.parseInt(populationV);
				String place = "village";
				if (population > 100000) {
					place = "city";
				} else if (population > 10000) {
					place = "town";
				}

				writer.append(String.format("%s %f %f %s", id, centroid.getX(),
						centroid.getY(), place));
				writer.newLine();

			} catch (ParserConfigurationException e) {
				System.out.println("error while reading file: "
						+ e.getMessage());
			} catch (SAXException e) {
				System.out.println("error while reading file: "
						+ e.getMessage());
			} catch (IOException e) {
				System.out.println("error while reading file: "
						+ e.getMessage());
			}
		}
		
		try {
			writer.close();
		} catch (IOException e) {
			System.out.println("unable to close writer");
			System.exit(1);
		}
	}

}
