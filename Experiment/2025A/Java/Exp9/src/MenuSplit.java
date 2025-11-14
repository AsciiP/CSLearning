public class MenuSplit {
    public static void main(String[] args) {
        String menu = "红烧肉:38元, 西红柿炒蛋:18元, 清蒸鱼:48元, 青菜:12元";

        String[] items = menu.split(",\\s*");
        int total = 0;

        System.out.println("菜单价格如下：");
        for (String item : items) {
            String[] parts = item.split(":");
            String name = parts[0];
            String priceStr = parts[1].replace("元", "");
            int price = Integer.parseInt(priceStr);
            System.out.println(name + " 价格：" + price + "元");
            total += price;
        }

        System.out.println("总价格：" + total + "元");
    }
}