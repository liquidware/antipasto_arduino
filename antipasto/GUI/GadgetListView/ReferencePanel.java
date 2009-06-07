package antipasto.GUI.GadgetListView;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Point;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;

import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;

import antipasto.GUI.GadgetListView.GadgetPanelEvents.ActiveGadgetObject;
import antipasto.GUI.GadgetListView.GadgetPanelEvents.IActiveGadgetChangedEventListener;
import antipasto.GUI.GadgetListView.GadgetPanelEvents.IActiveSketchChangingListener;
import antipasto.GUI.GadgetListView.GadgetPanelEvents.SketchChangingObject;
import antipasto.Interfaces.IModule;

import processing.app.Editor;

public class ReferencePanel extends JDialog implements ComponentListener, IActiveGadgetChangedEventListener, FocusListener{
	
	//The standard width and height for the dialog
	private int cachedHeight = 425;
    private int cachedWidth = 300;
	private JTextArea textArea;
	private JFrame component;
	private IModule activeModule;
	private JScrollPane scrollPane;
    
	public ReferencePanel(JFrame parent){
		super(parent, false);
		component = parent;
		this.setUndecorated(true);
		this.init();
		parent.addComponentListener(this);
		this.addComponentListener(this);
	}
	
	public void LoadText(String txt){
		this.textArea.setText(txt);
	}
	
	private void init(){
		JPanel topPanel = new JPanel();
		topPanel.setSize(cachedWidth, 15);
		topPanel.setBackground(new Color(0x04, 0x4F, 0x6F));
		
		JPanel bottomPanel = new JPanel();
		bottomPanel.setSize(cachedWidth, 15);
		bottomPanel.setBackground(new Color(0x04, 0x4F, 0x6F));
		
		this.getContentPane().setLayout(new BorderLayout());
		this.textArea = new JTextArea();
		
		this.setSize(cachedWidth, cachedHeight);
		this.setBackground(new Color(0x04, 0x4F, 0x6F));
		
		
		this.scrollPane = new JScrollPane(this.textArea);
		this.scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
		this.scrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		//this.scrollPane.add(this.textArea);
		
		this.textArea.setWrapStyleWord(true);
		this.textArea.setLineWrap(true);
		this.textArea.setBounds(this.scrollPane.getBounds());
		
		this.textArea.setVisible(true);
		this.scrollPane.setVisible(true);
		
		this.getContentPane().add(this.scrollPane, BorderLayout.CENTER);
		this.getContentPane().add(topPanel, BorderLayout.NORTH);
		this.getContentPane().add(bottomPanel, BorderLayout.SOUTH);
		
		this.textArea.addFocusListener(this);
		this.scrollPane.setVisible(true);
	}
	
	private void setLocation(){
		if(this.isVisible() && ((Editor)component).centerPanel.isVisible()){
			int xLocation = component.getX() + component.getWidth();
			int yLocation = ((Editor)component).centerPanel.getLocationOnScreen().y;
			this.setLocation(xLocation, yLocation);
		}
	}
	
	//Component Listener: This is designed to listen specifically to the editor windows so that we can adjust our size according to
	//it for symmetry...specifically with the gadget panel
	public void componentHidden(ComponentEvent arg0) {
		
	}

	public void componentMoved(ComponentEvent arg0) {
		setLocation();
	}

	public void componentResized(ComponentEvent arg0) {
		// TODO Auto-generated method stub
		
	}

	public void componentShown(ComponentEvent arg0) {
		setLocation();
	}

	public void onActiveGadgetChanged(ActiveGadgetObject obj) {
		if(obj != null){
			System.out.println("Active gadget chainging in the reference panel");
			this.activeModule = obj.getModule();
			if(this.activeModule.getReferenceText() != null || this.activeModule.getReferenceText().equalsIgnoreCase("")){
				this.LoadText(this.activeModule.getReferenceText());
			}else{
				System.out.println("No text found");
				this.textArea.setText("");
			}
		}else{
			this.textArea.setText("");
			System.out.println("Setting text to blank");
		}
	}

	public void focusGained(FocusEvent arg0) {
		
	}

	public void focusLost(FocusEvent arg0) {
		if(this.activeModule != null){
			this.activeModule.setReferenceText(this.textArea.getText());
			System.out.println("setting the text");
		}else{
			System.out.println("Text being set to null");
		}
	}
}
