
/* Server class */
import java.net.ServerSocket;
import java.net.Socket;

public class ServerThread extends Thread implements Runnable {

    private ServerSocket serverAddr;

    public ServerThread(ServerSocket server) {
        serverAddr = server;
    }

    public void run() {
        while (!suscriptor.exit) {

            try {

                Socket sc = serverAddr.accept();

                new ManageRequest(sc).start();
                
            } catch (Exception e) {
                System.err.println("c> NETWORK ERROR");
                break;
            }
        }
    }
}
