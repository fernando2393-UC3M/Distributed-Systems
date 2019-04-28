/* Server class */
import java.net.ServerSocket;
import java.net.Socket;

public class ServerThread extends Thread implements Runnable{

    private Socket sc;
    private ServerSocket serverAddr;

    public ServerThread(Socket socket, ServerSocket serversocket){
        sc = socket;
        serverAddr = serversocket;
    }

	public void run(){
		while(!suscriptor.exit){
            try {
                sc = serverAddr.accept();
                new ManageRequest(sc).start();
            }
            catch(Exception e){
                System.err.println("Exception " + e.toString());
            }
        }
	}
}