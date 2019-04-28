import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.Socket;

public class ManageRequest extends Thread implements Runnable {

    private Socket sc;

    public ManageRequest(Socket socket) {
        sc = socket;
    }

    public void run() {

        try {
            BufferedReader tp = new BufferedReader(new InputStreamReader(sc.getInputStream()));
            // saving
            String topicrec = tp.readLine();
            // Processing requests
            BufferedReader tx = new BufferedReader(new InputStreamReader(sc.getInputStream()));
            // saving
            String textrec = tx.readLine();

            System.out.println("c> MESSAGE FROM " + topicrec + " : " + textrec);

            sc.close(); // Message read --> Close socket connection
        } 
        catch (Exception e) {
            System.err.println("c> NETWORK ERROR");
        }
    }
}