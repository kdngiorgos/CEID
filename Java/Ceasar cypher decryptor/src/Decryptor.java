public class Decryptor {
    String ciphertext;

    public Decryptor(String kati){
        ciphertext = kati;
    }

    // Method to decrypt the message
    public void decrypt() {
        StringBuilder decryptedMessage = new StringBuilder();

        for (int shift =1;shift<27;shift++){
            for (int i = 0; i < ciphertext.length(); i++) {
                char c = ciphertext.charAt(i);

                if (Character.isLowerCase(c)) {
                    c = (char) ((c - 'a' - shift + 26) % 26 + 'a');
                }else if (Character.isUpperCase(c)) {
                    c = (char) ((c - 'A' - shift + 26) % 26 + 'A');
                }

                decryptedMessage.append(c);
            }

            String message = decryptedMessage.toString();
            decryptedMessage.setLength(0);
            System.out.println(message + "   Shift = " + shift);
        }
    }
}
