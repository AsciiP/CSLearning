import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class SwingTest {
    public static void main(String[] args) {
        SwingUtilities.invokeLater(SwingTest::createAndShowGUI);
    }

    private static void createAndShowGUI() {
        JFrame frame = new JFrame("SwingTest");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLayout(new BorderLayout());

        // North: text field and button
        JTextField textField = new JTextField(20);
        JButton button = new JButton("SUBMIT");
        JPanel northPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        northPanel.add(textField);
        northPanel.add(button);
        frame.add(northPanel, BorderLayout.NORTH);

        // Center: text area
        JTextArea textArea = new JTextArea(15, 40);
        textArea.setEditable(false);
        frame.add(new JScrollPane(textArea), BorderLayout.CENTER);

        // South: combo box
        String[] items = { "Option 1", "Option 2", "Option 3" };
        JComboBox<String> comboBox = new JComboBox<>(items);
        frame.add(comboBox, BorderLayout.SOUTH);

        // Action to handle input/display
        ActionListener showAction = e -> {
            String t = textField.getText();
            if (t != null) {
                if ("clear".equalsIgnoreCase(t.trim())) {
                    textArea.setText("");
                } else if (!t.trim().isEmpty()) {
                    textArea.append(t + System.lineSeparator());
                }
            }
            textField.requestFocusInWindow();
            textField.selectAll();
        };

        textField.addActionListener(showAction); // Enter key in text field
        button.addActionListener(showAction);

        comboBox.addActionListener(e -> {
            Object sel = comboBox.getSelectedItem();
            if (sel != null) {
                textArea.append(sel.toString() + System.lineSeparator());
            }
        });

        frame.pack();
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
    }
}