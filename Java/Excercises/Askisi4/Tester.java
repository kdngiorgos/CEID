abstract public class Tester{
    public static void main(String []args){
        Robot r1 = new Robot("CRX","FANUC",15,true,12.2f);
        RobVehicle rv1 = new RobVehicle("test1","ABB",2,25.4f,23);
        
        System.out.print("Robot" + r1.getName()+" of "+ r1.getManufacturer()+" is");
        if(!r1.isOnline()){System.out.print(" not");}
        System.out.println(" online for "+r1.getDays_of_usage()+" days with costs equals to " + r1.getCosts()+"M");
        
        System.out.print("Roboic Vehicle "+rv1.getName()+" of"+rv1.getManufacturer()+" is online for"+rv1.getDays_of_usage()+" days with costs equals to" + rv1.getCosts()+"M");
            if(rv1.getMiles()>0){System.out.println(" with traveled distance "+rv1.getMiles()+"miles");}
            else{System.out.println(" 0 miles traveled distance");}
    }
}
