import java.awt.BorderLayout;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.KeyStroke;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.filechooser.FileNameExtensionFilter;

public class EasyNotepad extends JFrame {
    private final JTextArea textArea = new JTextArea();
    private final JFileChooser fileChooser = new JFileChooser();
    private File currentFile = null;
    private boolean modified = false;

    public EasyNotepad() {
        super("Untitled - EasyNotepad");
        setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
        setSize(800, 600);
        initComponents();
        setVisible(true);
    }
    
    private void initComponents() {
        // Text area
        textArea.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 14));
        JScrollPane scrollPane = new JScrollPane(textArea);
        add(scrollPane, BorderLayout.CENTER);

        // textArea Document listener to track edits
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
        fileChooser.setFileFilter(new FileNameExtensionFilter("纯文本文件 (*.txt)", "txt"));

        // Menu
        JMenuBar menuBar = new JMenuBar();
        JMenu fileMenu = new JMenu("文件(F)");
        fileMenu.setMnemonic('F');
        
        JMenuItem newItem = new JMenuItem("新建(N)");
        newItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_N, InputEvent.CTRL_DOWN_MASK));
        newItem.setMnemonic('N');
        newItem.addActionListener(e -> newFile());

        JMenuItem openItem = new JMenuItem("打开(O)...");
        openItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_O, InputEvent.CTRL_DOWN_MASK));
        openItem.setMnemonic('O');
        openItem.addActionListener(e -> openFile());

        JMenuItem saveItem = new JMenuItem("保存(S)");
        saveItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S, InputEvent.CTRL_DOWN_MASK));
        saveItem.setMnemonic('S');
        saveItem.addActionListener(e -> save());

        JMenuItem saveAsItem = new JMenuItem("另存为(A)...");
        saveAsItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S, InputEvent
                .CTRL_DOWN_MASK | InputEvent.SHIFT_DOWN_MASK));
        saveAsItem.setMnemonic('A');
        saveAsItem.addActionListener(e -> saveAs());

        JMenuItem exitItem = new JMenuItem("退出(X)");
        exitItem.setMnemonic('X');
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

        // Hit X to close window
        addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                exitApplication();
            }
        });
    }

    private void setModified(boolean modified) {
        this.modified = modified;
        String title = (currentFile != null) ? currentFile.getName() : "Untitled";
        if (modified) {
            title += " *";
        }
        setTitle(title + " - EasyNotepad");
    }
}
