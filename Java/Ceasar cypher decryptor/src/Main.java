import java.util.*;

public class Main {
   public static void main(String[] args) {
       Scanner myObj = new Scanner(System.in);
       System.out.println("Enter the sentence you want to decrypt: ");
       String sentence = myObj.nextLine();
       Decryptor test = new Decryptor(sentence);

       test.decrypt();
   }
}