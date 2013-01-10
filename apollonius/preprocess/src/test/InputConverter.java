package test;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.Map;

import de.topobyte.osm.model.iface.EntityType;
import de.topobyte.osm.model.iface.INode;
import de.topobyte.osm.model.util.OsmModelUtil;
import de.topobyte.osm.tbo.access.EntityContainer;
import de.topobyte.osm.tbo.access.TboIterator;
import de.topobyte.osm.tbo.io.InputStreamCompactReader;

public class InputConverter {

	public static void main(String[] args) throws FileNotFoundException {
		String filename = args[0];

		TboIterator iterator = new TboIterator(new InputStreamCompactReader(
				new FileInputStream(filename)));
		while (iterator.hasNext()) {
			EntityContainer container = iterator.next();

			// skip non-nodes
			if (container.getType() != EntityType.Node) {
				continue;
			}
			INode node = (INode) container.getEntity();
			Map<String, String> tags = OsmModelUtil.getTagsAsMap(node);

			// skip non-place
			String place = tags.get("place");
			if (place == null) {
				continue;
			}

			// take only city, town or village
			if (!(place.equals("city") || place.equals("town") || place
					.equals("village"))) {
				continue;
			}

			// print data
			System.out.println(String.format("%d %f %f %s", node.getId(),
					node.getLongitude(), node.getLatitude(), place));
		}
	}
}
