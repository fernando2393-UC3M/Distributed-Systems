import java.io.*;
import java.lang.*;
import java.net.*;
import java.util.*;
import gnu.getopt.Getopt;

class suscriptor implements Runnable{

	/********************* TYPES **********************/

	/******************* ATTRIBUTES *******************/

	private static String _server = null;
	private static short _port = -1;
	private static int _serverport = -1;
	private static String topic = null;
	private static String unsub = null;

	/********************* METHODS ********************/
	//thread methods
	public suscriptor (String topic, int _serverport) {
	    this.topic = topic;
	    this._serverport = _serverport;
	  }
	public void run() throws RuntimeException {
		try {
			ServerSocket sc = new ServerSocket(_serverport);

			// infinite loop that will stop when user unsubscribes a topic
			while (!unsub.equals(topic)) { //boolean
				// Waiting for requests
				Socket serversock = sc.accept();
				// Processing requests
				BufferedReader tp = new BufferedReader(new InputStreamReader(serversock.getInputStream()));
				// saving
				String topicrec = tp.readLine();
				// Processing requests
				BufferedReader tx = new BufferedReader(new InputStreamReader(serversock.getInputStream()));
				// saving
				String textrec = tx.readLine();


				System.out.println("c> MESSAGE FROM " + topicrec + " : " + textrec);

				sc.close();
				serversock.close();
			}

		} catch (RuntimeException e) {
			System.err.println("c> NETWORK ERROR" + e.toString());
		} catch(IOException e) {
			System.err.println("Error in the connection to the broker " + _server + _serverport + e.toString());
		}
	}

	static int subscribe(String topic) {
		// Write your code here

		System.out.println("Subscribe to: " + topic);
		// Connection
		String host = _server;
		try {
			Socket sc = new Socket(host, _port);
			PrintWriter o = null;
			o = new PrintWriter(new OutputStreamWriter(sc.getOutputStream()));
			// choose port to listen
			ServerSocket s = new ServerSocket(0);//crear port 0, usara uno libre y hay que cogerlo de ahi
			_serverport = s.getLocalPort();
			String serport = String.valueOf(_serverport);
			// reply
			DataOutputStream os = new DataOutputStream(sc.getOutputStream());

			os.writeBytes("SUBSCRIBE\0");
			os.flush();

			os.writeBytes(serport + "\0");
			os.flush();

			os.writeBytes(topic + "\0");
			os.flush();



			BufferedReader in = null;
			String res = in.readLine();
			unsub = null;
			if (res.equals("-1")) {
				System.out.println("c> SUBSCRIBE FAIL");
				return -1;
			} else {
				System.out.println("c> SUBSCRIBE OK");
				Thread t = new Thread(new suscriptor(topic, _serverport));
				//prevent thread from exiting when the progamm finishes
				t.setDaemon(true);
				t.start();
			}
			sc.close();

		} catch (Exception e) {
			System.err.println("Error in the connection to the broker " + host + _port);
		}
		return 0;
	}

	static int unsubscribe(String topic) {
		// Write your code here
		System.out.println("Unsubscribe from: " + topic);
		// Connection
		String host = _server;
		try {
			Socket sc = new Socket(host, _port);
			DataOutputStream ostream = new DataOutputStream(sc.getOutputStream());
			// Send SUSCRIBE to server
			ostream.writeBytes("UNSUBSCRIBE" + "\0");
			// Send topic to server
			ostream.writeBytes(topic + "\0");
			// receive response
			DataInputStream istream = new DataInputStream(sc.getInputStream());
			BufferedReader r = new BufferedReader(new InputStreamReader(istream));
			String res = r.readLine();

			if (res.equals("2")) {
				System.out.println("c> UNSUBSCRIBE FAIL");
				sc.close();
				return -1;
				// topic was not subscibed
			} else if (res.equals("1")) {
				System.out.println("c> TOPIC NOT SUBSCRIBED");
				sc.close();
				return -1;
			} else if (res.equals("0")){
				System.out.println("c> UNSUBSCRIBE OK");
				sc.close();
				unsub = topic;
			}
		} catch (Exception e) {
			System.err.println("Error in the connection to the broker " + host + _port);
		}
		return 0;
	}

	/**
	 * @brief Command interpreter for the suscriptor. It calls the protocol
	 *        functions.
	 */
	static void shell() {
		boolean exit = false;
		String input;
		String[] line;
		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));

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
							exit = true;
						} else {
							System.out.println("Syntax error. Use: QUIT");
						}
					}

					/************* UNKNOWN ************/
					else {
						System.out.println("Error: command '" + line[0] + "' not valid.");
					}
				}
			} catch (java.io.IOException e) {
				System.out.println("Exception: " + e);
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
		System.out.println("+++ FINISHED +++");
	}
}
