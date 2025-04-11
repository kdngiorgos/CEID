public class Window
{
    private int size =5;
    public Window () {}
    
    public Window (int size){
        this.size=size ;}
    
        public int getSize(){
            return this.size ;
}
public static void main ( String args [ ] ){
Window w1 = new Window();
Window w2 = new Window(4);
Window w3 = new Window(6);
Window w4 = new Window();
System.out.println ("The size o f window w1 is : " + w1.getSize( )) ;
System.out.println ("The size o f window w2 is : " + w2.getSize( )) ;
System.out.println ("The size o f window w3 is : " + w3.getSize( )) ;
System.out.println ("The size o f window w4 is : " + w4.getSize( )) ;
}
}