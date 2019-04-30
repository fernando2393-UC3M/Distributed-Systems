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
            BufferedReader br = new BufferedReader(new InputStreamReader(sc.getInputStream()));
            
            String topic = br.readLine();

            String text = br.readLine();

            System.out.print("MESSAGE FROM " + topic + " : " + text);

            sc.close(); // Message read --> Close socket connection
        } 
        catch (Exception e) {
            System.err.print("NETWORK ERROR");
        }
    }
}