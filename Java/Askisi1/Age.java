
public class Age
{
    /* [V1] public static void main(String [] args){
        
        if (args.length == 1){
            
            System.out.println("Your age:");
            System.out.println(args [0]);
            System.out.println("Years Old.");
            
        }
        
    }

}*/

/* [V2] public static void main(String [] args){
        
        if (args.length == 1){
            
            System.out.print("You are ");
            System.out.print(args [0]);
            System.out.println(" years old.");
            
        } else System.out.println("You forgot to give your age.");
        
    }*/
    
    
    /* [V3] public static void main(String args[]){
        
        if (args.length == 1){
            
            System.out.print("You are ");
            System.out.print(args [0]);
            System.out.println(" years old.");
            
        } else if (args.length == 0){
            
            System.out.println("You forgot to give your age.");
            
        } else { 
            
            System.out.print(args[1]);
            System.out.print("is:");
            System.out.println(args[0]);
            System.out.println("years old");
        } */
        //Exe.6: To adjust the main so that the name can also be printed, switch the System.out.prinln("Your age") to your name (is) command cotent to:
        
        /* [V4] public static void main(String args[]){
        
        if (args.length == 1){
            
            System.out.print("You are " + args[0] + " years old.");
            
        } else if (args.length == 0){
            
            System.out.println("You forgot to give your age.");
            
        } else { 
            System.out.print(args[1] + " is " + args[0] + " years old.");
        }
    }*/
    //Exe 7. The syntax and text is now more organised
}
