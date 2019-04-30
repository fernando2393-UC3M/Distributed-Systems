import java.io.*;
import java.net.*;
import gnu.getopt.Getopt;

class suscriptor{

	/********************* TYPES **********************/

	/******************* ATTRIBUTES *******************/

	private static String _server = null;
	private static int _port = -1;

	public static boolean exit = false;

	private static int serverport = -1;

	private static ServerSocket serverAddr = null;

	static int listenport = -1;

	/********************* METHODS ********************/

	static void createServer(){
		/* Server creation for listening to new messages from subscribed topics */

		try {
			serverAddr = new ServerSocket(0); // Random free port
		} catch (Exception e) {
			System.err.println("c> NETWORK ERROR");
		}

		serverport = serverAddr.getLocalPort(); // Get port from server for posterior messages		

		/* Initialize server socket for listening to broker */

		new ServerThread(serverAddr).start();
		
	}

	static int subscribe(String topic) {
		// Write your code here

		if (topic.length() > 128) {

			System.err.println("c> Topic has more than 128 characters");

			return -1;

		}

		// Connection
		String host = _server;

		try {

			Socket sc = new Socket(host, _port);

			DataOutputStream os = new DataOutputStream(sc.getOutputStream());
			BufferedReader istream = new BufferedReader(new InputStreamReader(sc.getInputStream()));

     /* Sending message */

			os.writeBytes("SUBSCRIBE" + "\0");
			os.flush();

			os.writeBytes(topic + "\0");
			os.flush();

			/* Send to the broker the port where communications must be received */

			os.writeBytes(serverport + "\0");
			os.flush();

			/* Receiving message */

			String res = istream.readLine();

			if (res.equals("1\0")) {

				System.out.println("c> SUBSCRIBE FAIL");

			}

			else if(res.equals("0\0")) {

				System.out.println("c> SUBSCRIBE OK");

			}

			sc.close();

		} catch (Exception e) {
			System.err.println("Error in the connection to the broker " + host + " : " + _port);
		}
		return 0;
	}

	static int unsubscribe(String topic) {
		// Write your code here

		if (topic.length() > 128) {

			System.err.println("c> Topic has more than 128 characters");

			return -1;

		}

		// Connection
		String host = _server;

		try {

			Socket sc = new Socket(host, _port);

			DataOutputStream os = new DataOutputStream(sc.getOutputStream());
			BufferedReader istream = new BufferedReader(new InputStreamReader(sc.getInputStream()));

			/* Sending message */

			os.writeBytes("UNSUBSCRIBE" + "\0");
			os.flush();

			os.writeBytes(topic + "\0");
			os.flush();

			/* Send to the broker the port where communications must be received */

			os.writeBytes(serverport + "\0");
			os.flush();

			/* Receiving message */

			String res = istream.readLine();

			if (res.equals("2\0")) {

				System.out.println("c> UNSUBSCRIBE FAIL");

			}

			else if (res.equals("1\0")) {

				System.out.println("c> TOPIC NOT SUBSCRIBED");

			}

			else if (res.equals("0\0")) {

				System.out.println("c> UNSUBSCRIBE OK");

			}

			sc.close();

		} catch (Exception e) {
			System.err.println("Error in the connection to the broker " + host + " : " + _port);
		}
		return 0;
	}

	/**
	 * @brief Command interpreter for the suscriptor. It calls the protocol
	 *        functions.
	 */
	static void shell() {

		String input;
		String[] line;
		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));

		/* Create server to receive messages for each topic */

		createServer();

		while (!exit) {
			try {

				System.out.print("c> ");
				input = in.readLine();
				line = input.split("\\s");

				if (line.length > 0) {
					/*********** SUBSCRIBE *************/
					if (line[0].equals("SUBSCRIBE")) {
						if (line.length == 2) {
							subscribe(line[1]); // topic = line[1]
						} else {
							System.out.println("Syntax error. Usage: SUBSCRIBE <topic>");
						}
					}

					/********** UNSUBSCRIBE ************/
					else if (line[0].equals("UNSUBSCRIBE")) {
						if (line.length == 2) {
							unsubscribe(line[1]); // topic = line[1]
						} else {
							System.out.println("Syntax error. Usage: UNSUBSCRIBE <topic>");
						}
					}

					/************** QUIT **************/
					else if (line[0].equals("QUIT")) {
						if (line.length == 1) {
							serverAddr.close();
							exit = true;
						} else {
							System.out.println("Syntax error. Usage: QUIT");
						}
					}

					/************* UNKNOWN ************/
					else {
						System.out.println("Error: command '" + line[0] + "' not valid.");
					}
				}
			} catch (java.io.IOException e) {
				System.err.println("c> NETWORK ERROR");
				e.printStackTrace();
			}
		}		
	}

	/**
	 * @brief Prints program usage
	 */
	static void usage() {
		System.out.println("Usage: java -cp . suscriptor -s <server> -p <port>");
	}

	/**
	 * @brief Parses program execution arguments
	 */
	static boolean parseArguments(String[] argv) {
		Getopt g = new Getopt("suscriptor", argv, "ds:p:");

		int c;
		String arg;

		while ((c = g.getopt()) != -1) {
			switch (c) {
			// case 'd':
			// _debug = true;
			// break;
			case 's':
				_server = g.getOptarg();
				break;
			case 'p':
				arg = g.getOptarg();
				_port = Short.parseShort(arg);
				break;
			case '?':
				System.out.print("getopt() returned " + c + "\n");
				break; // getopt() already printed an error
			default:
				System.out.print("getopt() returned " + c + "\n");
			}
		}

		if (_server == null)
			return false;

		if ((_port < 1024) || (_port > 65535)) {
			System.out.println("Error: Port must be in the range 1024 <= port <= 65535");
			return false;
		}

		return true;
	}

	/********************* MAIN **********************/

	public static void main(String[] argv) {
		if (!parseArguments(argv)) {
			usage();
			return;
		}

		// Write code here

		shell();
	}
}
