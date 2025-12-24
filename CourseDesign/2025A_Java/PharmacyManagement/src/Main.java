import com.formdev.flatlaf.FlatLightLaf;
import javax.swing.UIManager;
import gui.LoginFrame;
import java.awt.Font;

public class Main {
    public static void main(String[] args) {
        try {
            UIManager.setLookAndFeel(new FlatLightLaf());
            UIManager.put("defaultFont", new Font("Microsoft YaHei", Font.PLAIN, 14));
        } catch (Exception e) {
            System.err.println("Failed to initialize FlatLaf");
        }

        new LoginFrame().setVisible(true);
    }
}
