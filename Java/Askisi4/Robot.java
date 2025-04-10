public class Robot {
    private String name ;
    private String manufacturer ;
    private int days_of_usage ;
    private boolean online ;
    private float costs ;
    
    public Robot( String name , String manufacturer , int days_of_usage , boolean online , float costs ){
        this.name = new String (name);
        this.manufacturer = new String (manufacturer);
        this.days_of_usage = days_of_usage ;
        this.online = online;
        this.costs = costs;}
    
    public String getName (){return name;}
    public String getManufacturer(){return manufacturer;}
    public boolean isOnline(){return online; }
    public float getCosts(){return costs;}
    public int getDays_of_usage(){return days_of_usage;}
}