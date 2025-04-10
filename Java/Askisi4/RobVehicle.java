public class RobVehicle extends Robot{
    private int miles ;
    
    public RobVehicle(String name,String manufacturer,int days_of_usage,float costs,int miles ){
        super(name,manufacturer,days_of_usage,true, costs) ;
        this.miles = miles;
    }
    
        public int getMiles (){
            return miles ;
        }
}
