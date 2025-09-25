import java.util.Scanner;
import java.util.Formatter;

public class Main {
    public static void main(String[] args) {
        Teylor teylor = new Teylor();
        Scanner sc = new Scanner(System.in);

        double x;
        int k;
        System.out.println("Введите x");
        x = sc.nextDouble();
        if (x < -1 || x > 1) {
        System.out.println("Введено неккоректное число");
        return ;
        }
        System.out.println("Введите k");
        k = sc.nextInt();

        teylor.calculate(x, k);
        double res = teylor.getSum();
        int rounded = (int)Math.round(res);

        Formatter formatter = new Formatter();
        formatter.format("Результат: %05." + (k + 1) + "f%n", res);
        formatter.format("Округлённое значение в восьмеричном виде: %#o%n", rounded);
        formatter.format("Округлённое значение в шестнадцатеричном виде: %#x%n", rounded);
        System.out.println(formatter);
        formatter.close();

        double standart, del;
        del = (1 + x)/(1 - x);
        standart = Math.log(del);
        System.out.println("Стандартный вывод: " + standart);
    }
}
