package swp12.apollonius.evaluation;

import java.io.BufferedInputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;

public class ReadDoubles {
	public static void main(String[] args) throws FileNotFoundException {
		if (args.length != 1) {
			System.out.println("usage: " + ReadDoubles.class.getSimpleName()
					+ " <file>");
			System.exit(1);
		}

		String filename = args[0];

		FileInputStream fis = new FileInputStream(filename);
		BufferedInputStream is = new BufferedInputStream(fis);
		while (true) {
			try {
				double lon = readDouble(is);
				double lat = readDouble(is);
				System.out.println(lon);
				System.out.println(lat);
			} catch (IOException e) {
				break;
			}
		}

	}

	private static double readDouble(BufferedInputStream is) throws IOException {
		long bits = 0;
		bits |= ((long)readByte(is)) << 56;
		bits |= ((long)readByte(is)) << 48;
		bits |= ((long)readByte(is)) << 40;
		bits |= ((long)readByte(is)) << 32;
		bits |= ((long)readByte(is)) << 24;
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

}
