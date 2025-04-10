import java.io.*;

public class IO_Tester{
    public static int readInt(){
        byte b[] = new byte[16];
        String str;
        try{
        System.in.read(b);
        str = (new String(b)).trim();
        return Integer.parseInt(str);}
        catch(IOException e){
            System.out.println("Exception:" + e);
            return 0;
        }
    }

    public static float readFloat() {
        byte[] b = new byte[16]; 
        String str;
        try {
            System.in.read(b);
            str = (new String(b)).trim();
            return Float.parseFloat(str); 
        } catch (IOException e) {
            System.out.println("Exception: " + e);
            return 0;
        } catch (NumberFormatException e) {
            System.out.println("Invalid float format");
            return 0;
        }
    }

    public static String readString() {
        byte[] b = new byte[32]; 
        String str;
        try {
            System.in.read(b);
            str = (new String(b)).trim();
            return str; 
        } catch (IOException e) {
            System.out.println("Exception: " + e);
            return ""; 
        }
    }

    public static boolean readBoolean() {
        byte[] b = new byte[16];
        String str;
        try {
            System.in.read(b);
            str = (new String(b)).trim();
            return Boolean.parseBoolean(str);
        } catch (IOException e) {
            System.out.println("Exception: " + e);
            return false;
        }
    }

    public static void main(String[] args) {
        System.out.println("Dwste enan akeraio:");
        int i = readInt();
        System.out.println("Dwste enan pragmatiko:");
        float f = readFloat();
        System.out.println("i="+i+"\t f="+f);}}