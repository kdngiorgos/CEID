public class MWindow extends Window{
    private String message = "No message";
    protected int size =7;
    
    public MWindow(String message){
        size =2;
        this.message = message;
        System.out.println("Window message = " + message);
    }
    
    public MWindow(int size, String message){
        super(size);
        this.message = message;
        System.out.println("Window message = " + message);
    }
    
    public void setSize1(int y){size = y;}
    public void setSize2(int y){super.setSize(y);}
    public void printSize(){System.out.println("MSize= " + size);}
    public void printSize1(){System.out.println(super.size);}
    public void printSize2(){super.printSize();}
}