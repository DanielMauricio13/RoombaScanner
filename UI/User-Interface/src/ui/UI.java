package ui;

import com.sun.tools.javac.Main;

import javax.swing.*;

/**
 * @author Coby Konkol
 */

public class UI {
    private int width, height;
    private MainPanel mainPanel;
    private JFrame jFrame;

    public UI(int width, int height){

        this.width = width;
        this.height = height;


        jFrame = new JFrame("Cybot_Name");
        jFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        jFrame.setSize(width, height);


        mainPanel = new MainPanel();
        jFrame.setVisible(true);
    }
}
