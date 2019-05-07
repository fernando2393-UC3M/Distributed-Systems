import java.math.BigInteger;
import java.net.URL;
import javax.xml.namespace.QName;
import javax.xml.ws.Service;

import com.dataaccess.webservicesserver.NumberConversion;
import com.dataaccess.webservicesserver.NumberConversionSoapType;
import com.dataaccess.webservicesserver.NumberToWords;
import com.dataaccess.webservicesserver.NumberToWordsResponse;

import javax.jws.WebService;

public class formatter {

        public String converter(String text) {

        String [] words = text.split("\\s+");

        NumberConversion myservice = new NumberConversion();
        NumberConversionSoapType port = myservice.getNumberConversionSoap();
        String finalText = "";

        for (int i = 0; i < words.length; i++){

            if(isNumeric(words[i])) { // The introduced String is a number
                BigInteger ubiNum = new BigInteger(words[i]);
                words[i] = port.numberToWords(ubiNum);
                finalText += words[i];
            }
            else {
                finalText += words[i] + " ";
            }
        }

        return finalText;

    }


    public static boolean isNumeric(String str) {
        try {
            Double.parseDouble(str);
            return true;
        } catch (NumberFormatException e) {
            return false;
        }
    }

}