import java.io.BufferedReader;
import java.util.Scanner;
import java.io.InputStreamReader;
import java.io.IOException;
import java.math.BigDecimal;
import java.util.Formatter;
import java.util.Scanner;

public class MainBig {
    public static void main(String[] args) throws IOException {
        Scanner sc = new Scanner(System.in);


        TeylorBig teylor = new TeylorBig();
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));

        System.out.println("Введите x (от -1 до 1):");
        BigDecimal x = new BigDecimal(reader.readLine());
        if (x.compareTo(BigDecimal.ONE) > 0 || x.compareTo(BigDecimal.valueOf(-1)) < 0) {
            System.out.println("Введено некорректное число");
            return;
        }

        System.out.println("Введите k:");
        int k = Integer.parseInt(reader.readLine());

        teylor.calculate(x, k);

        Formatter formatter = new Formatter();
        formatter.format("Результат: %0#"+(k+2)+"."+ (k+1) +"f", teylor.getSum());
        System.out.println(formatter);

        int rounded = teylor.getSum().setScale(0, BigDecimal.ROUND_HALF_UP).intValue();
        System.out.printf("Округлённое значение: %d\n", rounded);
        System.out.printf("В восьмеричной системе: %#o\n", rounded);
        System.out.printf("В шестнадцатеричной системе: %#x\n", rounded);

        formatter.close();

        double standart, del, y;
        System.out.println("Введите y");
        y = sc.nextDouble();
        del = (1 + y)/(1 - y);
        standart = Math.log(del);
        System.out.println("Стандартный вывод: " + standart);
    }
    public static TeylorBig runCalculation(BufferedReader reader) throws IOException {
        TeylorBig teylor = new TeylorBig();

        BigDecimal x = new BigDecimal(reader.readLine());
        x.doubleValue();
        if (x.compareTo(BigDecimal.ONE) > 0 || x.compareTo(BigDecimal.valueOf(-1)) < 0) {
            throw new IllegalArgumentException("Введено некорректное число");
        }

        int k = Integer.parseInt(reader.readLine());
        teylor.calculate(x, k);
        return teylor;
    }
}
