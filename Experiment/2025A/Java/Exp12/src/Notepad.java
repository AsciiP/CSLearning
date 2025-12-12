import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.nio.charset.StandardCharsets;
import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.filechooser.FileNameExtensionFilter;

public class Notepad extends JFrame {
    private final JTextArea textArea = new JTextArea();
    private final JFileChooser fileChooser = new JFileChooser();
    private File currentFile = null;
    private boolean modified = false;

    public Notepad() {
        super("Untitled - Notepad");
        setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
        setSize(800, 600);
        initComponents();
        setLocationRelativeTo(null);
        setVisible(true);
    }

    private void initComponents() {
        // Text area
        textArea.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 14));
        JScrollPane scrollPane = new JScrollPane(textArea);
        add(scrollPane, BorderLayout.CENTER);

        // Document listener to track modifications
        textArea.getDocument().addDocumentListener(new DocumentListener() {
            @Override
            public void insertUpdate(DocumentEvent e) {
                setModified(true);
            }
            
            @Override
            public void removeUpdate(DocumentEvent e) {
                setModified(true);
            }
            @Override
            public void changedUpdate(DocumentEvent e) {
                setModified(true);
            }
        });

        // File chooser filter
        fileChooser.setFileFilter(new FileNameExtensionFilter("Text Files (*.txt)", "txt"));
        fileChooser.setAcceptAllFileFilterUsed(true);

        // Menu
        JMenuBar menuBar = new JMenuBar();
        JMenu fileMenu = new JMenu("File");

        JMenuItem newItem = new JMenuItem("New");
        newItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_N, InputEvent.CTRL_DOWN_MASK));
        newItem.addActionListener(e -> newFile());

        JMenuItem openItem = new JMenuItem("Open...");
        openItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_O, InputEvent.CTRL_DOWN_MASK));
        openItem.addActionListener(e -> openFile());

        JMenuItem saveItem = new JMenuItem("Save");
        saveItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S, InputEvent.CTRL_DOWN_MASK));
        saveItem.addActionListener(e -> save());

        JMenuItem saveAsItem = new JMenuItem("Save As...");
        saveAsItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S, InputEvent.CTRL_DOWN_MASK | InputEvent.SHIFT_DOWN_MASK));
        saveAsItem.addActionListener(e -> saveAs());

        JMenuItem exitItem = new JMenuItem("Exit");
        exitItem.addActionListener(e -> exitApplication());

        fileMenu.add(newItem);
        fileMenu.add(openItem);
        fileMenu.addSeparator();
        fileMenu.add(saveItem);
        fileMenu.add(saveAsItem);
        fileMenu.addSeparator();
        fileMenu.add(exitItem);

        menuBar.add(fileMenu);
        setJMenuBar(menuBar);

        // Window closing
        addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                exitApplication();
            }
        });
    }

    private void setModified(boolean flag) {
        if (modified == flag) return;
        modified = flag;
        updateTitle();
    }

    private void updateTitle() {
        String name = (currentFile == null) ? "Untitled" : currentFile.getName();
        setTitle((modified ? "*" : "") + name + " - Notepad");
    }

    private void newFile() {
        if (!confirmDiscardChanges()) return;
        currentFile = null;
        textArea.setText("");
        setModified(false);
    }

    private void openFile() {
        if (!confirmDiscardChanges()) return;
        int result = fileChooser.showOpenDialog(this);
        if (result != JFileChooser.APPROVE_OPTION) return;
        File file = fileChooser.getSelectedFile();
        readFile(file);
    }

    private void readFile(File file) {
        try (BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(file), StandardCharsets.UTF_8))) {
            StringBuilder sb = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) {
                sb.append(line).append('\n');
            }
            textArea.setText(sb.toString());
            currentFile = file;
            setModified(false);
        } catch (IOException ex) {
            JOptionPane.showMessageDialog(this, "Failed to open file:\n" + ex.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
        }
    }

    private void save() {
        if (currentFile == null) {
            saveAs();
        } else {
            writeFile(currentFile);
        }
    }

    private void saveAs() {
        int result = fileChooser.showSaveDialog(this);
        if (result != JFileChooser.APPROVE_OPTION) return;
        File file = fileChooser.getSelectedFile();
        // Ensure .txt extension if user selected text filter and no extension provided
        if (!file.getName().contains(".") && fileChooser.getFileFilter() instanceof FileNameExtensionFilter) {
            file = new File(file.getAbsolutePath() + ".txt");
        }
        writeFile(file);
    }

    private void writeFile(File file) {
        try (BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(file), StandardCharsets.UTF_8))) {
            writer.write(textArea.getText());
            currentFile = file;
            setModified(false);
        } catch (IOException ex) {
            JOptionPane.showMessageDialog(this, "Failed to save file:\n" + ex.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
        }
    }

    private boolean confirmDiscardChanges() {
        if (!modified) return true;
        String[] options = {"Save", "Don't Save", "Cancel"};
        int choice = JOptionPane.showOptionDialog(this,
                "The document has unsaved changes. Do you want to save them?",
                "Unsaved Changes",
                JOptionPane.DEFAULT_OPTION,
                JOptionPane.WARNING_MESSAGE,
                null,
                options,
                options[0]);
        switch (choice) {
            case 0 -> {
                // Save
                save();
                return !modified; // if save succeeded, modified == false
            }
            case 1 -> {
                // Don't Save
                return true;
            }
            default -> {
                // Cancel or closed
                return false;
            }
        }
    }

    private void exitApplication() {
        if (!confirmDiscardChanges()) return;
        dispose();
        System.exit(0);
    }

    public static void main(String[] args) {
        // Ensure nice look and feel
        try {
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        } catch (Exception ignored) { }
        SwingUtilities.invokeLater(Notepad::new);
    }
}