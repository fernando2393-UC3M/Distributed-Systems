import java.io.*;
import java.lang.*;
import java.net.*;
import java.util.*;
import gnu.getopt.Getopt;

public class ManageRequest extends Thread implements Runnable {

    private Socket sc;

    public ManageRequest(Socket socket) {
        sc = socket;
    }

    public void run() throws RuntimeException {

        try {
            BufferedReader tp = new BufferedReader(new InputStreamReader(sc.getInputStream()));
            // saving
            String topicrec = tp.readLine();
            // Processing requests
            BufferedReader tx = new BufferedReader(new InputStreamReader(sc.getInputStream()));
            // saving
            String textrec = tx.readLine();

            System.out.println("c> MESSAGE FROM " + topicrec + " : " + textrec);

            sc.close();
        }

        catch (RuntimeException e) {
            System.err.println("c> NETWORK ERROR" + e.toString());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}