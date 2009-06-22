package antipasto.GUI.GadgetListView;

import java.awt.BorderLayout;
import java.awt.Button;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.ContainerEvent;
import java.awt.event.ContainerListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.font.FontRenderContext;
import java.awt.geom.Line2D;
import java.awt.geom.Rectangle2D;
import java.awt.geom.RoundRectangle2D;
import java.io.File;
import java.io.FilenameFilter;
import java.io.IOException;
import java.util.List;

import javax.swing.BoxLayout;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.ScrollPaneLayout;

import antipasto.ScriptRunner;
import antipasto.GUI.GadgetListView.GadgetPanelEvents.ActiveGadgetObject;
import antipasto.GUI.GadgetListView.GadgetPanelEvents.IActiveGadgetChangedEventListener;
import antipasto.GUI.GadgetListView.GadgetPanelEvents.IActiveSketchChangingListener;
import antipasto.GUI.GadgetListView.GadgetPanelEvents.SketchChangingObject;
import antipasto.GUI.ImageListView.ScriptCellRenderer;
import antipasto.Interfaces.IModule;
import antipasto.Util.GadgetFileFilter;
import antipasto.Util.ScriptFileFilter;
import antipasto.Util.Utils;
import processing.app.*;

public class Wing extends JDialog implements ComponentListener,
		IActiveGadgetChangedEventListener, FocusListener, ISelectedItemListener {

	// The standard width and height for the dialog
	private int cachedHeight = 425;
	private int cachedWidth = 300;
	
	private JFrame component;
	private IModule activeModule;

	private JPanel	   wingHeader;
	private JPanel	   wingBody;
	private WingFooter wingFooter;
	
	private WingPanelScripts   wingPanelScripts;
	private WingPanelReference wingPanelReference;
	
	private String[]  wingTabNames = {""};
	private WingTab[] wingTabs   = {null,null,null,null};
	private JPanel[]  wingPanels = {null,null,null,null};
	private int		  wingFocusedIndex = 0;
	
	public Wing(JFrame parent) {
		super(parent, false);
		component = parent;
		this.setUndecorated(true);
		this.init();
		parent.addComponentListener(this);
		this.addComponentListener(this);
	}
	
	/**
	 * Initialize a Wing Header
	 *  Returns: a panel with the Tabs populated for the right wing.
	 */
	JPanel initWingHeader(String[] tabNames) {

		JPanel bgPanel = new JPanel();
		bgPanel.setSize(cachedWidth, 15);
		bgPanel.setBackground(new Color(0x21, 0x68, 0x86));
		bgPanel.setLayout(new BorderLayout());
		
		/* Create the tab Panel */
		JPanel tabPanel = new JPanel();
		tabPanel.setBackground(new Color(0x21, 0x68, 0x86));
		tabPanel.setSize(cachedWidth, 15);
		tabPanel.setLayout(new FlowLayout(FlowLayout.LEFT,0,0));
		tabPanel.setOpaque(true);
		
		/* Create the tabs */
		wingTabNames = tabNames.clone();
		for (int x=0; x<wingTabNames.length; x++) {
			wingTabs[x] = new WingTab(wingTabNames[x],null);
			wingTabs[x].addSelectionListener(this);
			tabPanel.add(wingTabs[x]);
		}
		
		/* A Horizontal Separator */
		JComponent hrImg = new JComponent() {
									private static final long serialVersionUID = 1L;

									public void paint(Graphics g) {
							 	        Graphics2D graphics2 = (Graphics2D) g;
							 	        graphics2.setBackground(new Color(255,255,255));
							 	        graphics2.setColor(new Color(255,255,255));
							 	        graphics2.drawLine(0,0,700,1);
							       	 }};
		
		bgPanel.add(tabPanel, BorderLayout.NORTH);					       	 
		bgPanel.add(hrImg, BorderLayout.CENTER);	
		
		return bgPanel;
	}
	
	/*************************************************
	 * Initialize a Test Panel
	 *  Returns: The test panel
	 */
	JPanel initTestPanel(String message) {

		JPanel panel = new JPanel();
		panel.setBackground(new Color(255, 255, 255));
		panel.setLayout(new BorderLayout());
		panel.setOpaque(true);
		
		JLabel label = new JLabel(message);
		label.setForeground(new Color(0, 0, 0));
		
		JPanel headerPanel = initTestPanelHeader(message);
		
		// Assemble the panel
		panel.add(headerPanel, BorderLayout.NORTH);
		panel.add(label, BorderLayout.CENTER);
		return panel;
	}
	

	/**
	 * Initialize a Test Panel Header
	 *  Returns: a JPanel with a header text 
	 */
	JPanel initTestPanelHeader(String textDisplay) {
		
		JPanel headerPanel = new JPanel();
		headerPanel.setBackground(new Color(0x04, 0x4F, 0x6F));
		headerPanel.setLayout(new BorderLayout());

		JLabel headerLabel = new JLabel(textDisplay);
		
		headerLabel.setForeground(new Color(0xFF, 0xFF, 0xFF));
		headerPanel.add(headerLabel, BorderLayout.CENTER);		
		
		return headerPanel;
	}
	

	/*************************************************
	 * Initialize the Wing Body
	 * 
	 * The Wing Body always returns:     --------------
	 * 								      Header Panel
	 * 						     	      Content Panel
	 *								     ---------------
	 */ 								
	JPanel initWingBody() {
		
		
		  /* Build each panel */
		  wingPanelReference =  new WingPanelReference("This is the reference panel", 
							    						wingFooter,
							    						cachedWidth,
							    						cachedHeight);
		  
		  String scriptPath = new String(Sketchbook.getSketchbookPath() + File.separator); 
		  wingPanelScripts = new WingPanelScripts(	scriptPath, 
				  							   		wingFooter, 
				  							   		cachedWidth, 
				  							   		cachedHeight);
		  

		  wingPanels[0] = wingPanelReference;
		  wingPanels[1] = wingPanelScripts;
		  wingPanels[2] = initTestPanel("Test1");
		  wingPanels[3] = initTestPanel("Test2");
		  
		  /* Retrieve the previous focused index */
		try {
			wingFocusedIndex = Preferences.getInteger("editor.rightWing.selectedTabIndex");
		} catch (Exception e) {
			wingFocusedIndex = 0;
		}
		  wingTabs[wingFocusedIndex].setFocused(true); //focus the tab
		  
		  return wingPanels[wingFocusedIndex];
		  
	}

	/*************************************************
	 * Initialize the Right Wing
	 * 
	 * The Right Wing always has:   --------------
	 * 								  Tab Panel
	 * 						     	  Content Panel
	 * 							      Status Panel
	 * 								---------------
	 */
	private void init() {

		this.getContentPane().setLayout(new BorderLayout());
		this.setSize(cachedWidth, cachedHeight);
		this.setBackground(new Color(0x04, 0x4F, 0x6F));
		
		
		wingHeader = initWingHeader(new String[] {"Reference", "Scripts", "Test 1", "Test 2"});
		wingFooter = new WingFooter(" Scripts loaded.", cachedWidth, 15);
		wingBody   = initWingBody();
		
		
		this.getContentPane().add(wingBody, BorderLayout.CENTER);
		this.getContentPane().add(wingHeader, BorderLayout.NORTH);
		this.getContentPane().add(wingFooter, BorderLayout.SOUTH);

	}

	
	private void setLocation() {
		if (this.isVisible() && ((Editor) component).centerPanel.isVisible()) {
			int xLocation = component.getX() + component.getWidth();
			int yLocation = ((Editor) component).centerPanel
					.getLocationOnScreen().y;
			this.setLocation(xLocation, yLocation);
		}
	}

	// Component Listener: This is designed to listen specifically to the editor
	// windows so that we can adjust our size according to
	// it for symmetry...specifically with the gadget panel
	public void componentHidden(ComponentEvent arg0) {

	}

	public void componentMoved(ComponentEvent arg0) {
		setLocation();
		repaint();
	}

	public void componentResized(ComponentEvent arg0) {
		// TODO Auto-generated method stub

	}

	public void componentShown(ComponentEvent arg0) {
		setLocation();
		repaint();
	}

	public void onActiveGadgetChanged(ActiveGadgetObject obj) {
		if (obj != null) {
			this.activeModule = obj.getModule();
			if (this.activeModule.getReferenceText() != null
					|| this.activeModule.getReferenceText()
							.equalsIgnoreCase("")) {
				
				// Disabled until reference text is written..
				// wingPanelReference.setText(this.activeModule.getReferenceText());
				wingPanelReference.setText(this.activeModule.getName() + " reference text here.");
			} else {
				System.out.println("No reference text found, defaulting ");
				wingPanelReference.setText(this.activeModule.getName() + " reference text here.");
			}
		} else {
			wingPanelReference.setText("Reference text area.");
		}
	}

	public void focusGained(FocusEvent arg0) {

	}

	public void focusLost(FocusEvent arg0) {
		
		/* Disabled until Omar needs this ;-)
		if (this.activeModule != null) {
			this.activeModule.setReferenceText(wingPanelReference.getText());
			System.out.println("setting the text");
		} else {
			System.out.println("Text being set to null");
		}
		*/
	}

	/**
	 * Tab Selection Control. 
	 * This event handles the tab and panel selection changes.
	 */
	public void onSelected(Object object) {
		// TODO Auto-generated method stub
		WingTab selected = (WingTab)object;
		
		/* A tab was selected, update the focus of all items */
		for (int x=0; x<wingTabs.length; x++) {
			
			if (wingTabs[x].txt == selected.txt) {

				/* Enable the other tabs and panels */
				wingFocusedIndex = x;			//Store the focused Id
				Preferences.setInteger("editor.rightWing.selectedTabIndex", wingFocusedIndex);
				wingTabs[x].setFocused(true);
				
				/* Load a new Body panel */
				wingBody = wingPanels[x];
				this.getContentPane().add(wingBody, BorderLayout.CENTER);
				wingPanels[x].setEnabled(true);
				wingPanels[x].setVisible(true);
				
			} else {
				
				 /* Disable the other tabs and Body panels */
				 wingTabs[x].setFocused(false);
				 wingPanels[x].setEnabled(false);
				 wingPanels[x].setVisible(false);
			}
			
		}
		
	}
	

}
