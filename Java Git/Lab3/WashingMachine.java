import java.util.Date;
import java.text.SimpleDateFormat;
import java.io.Serializable;

public class WashingMachine extends Fabrica implements Identifiable, Serializable {
    private static final long serialVersionUID = 1L;

    private int id;
    private String type;
    private String model;
    private String power;
    private String speed;
    private Date date;
    private String price;

    public WashingMachine(int id, String type, String model, String power, String speed, Date date, String price ) {
        this.id = id;
        this.type = type;
        this.model = model;
        this.power = power;
        this.speed = speed;
        this.date = date;
        this.price = price;
    }

    @Override
    public int getId() {
        return id;
    }
    @Override
    public void setId(int id) {
        this.id = id;
    }

    public String getType() {
        return type;
    }

    public String getModel() {
        return model;
    }

    public String getPower() {
        return power;
    }

    public String getSpeed() {
        return speed;
    }

    public Date getDate() {
        return date;
    }

    public String getPrice() {
        return price;
    }

    @Override
    public String toString() {
        SimpleDateFormat format = new SimpleDateFormat("dd-MM-yyyy");
        return  "id = " + id + '\n' +
                "type = " + type + '\n' +
                "model = " + model + '\n' +
                "power = " + power + '\n' +
                "speed = " + speed + '\n' +
                "date = " + format.format(date) + '\n' +
                "price = " + price + '\n';
    }
}
