package antipasto.GUI.GadgetListView;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.font.FontRenderContext;
import java.awt.geom.Rectangle2D;
import java.awt.geom.RoundRectangle2D;
import java.awt.image.BufferedImage;
import java.io.*;
import java.net.*;
import java.util.Iterator;
import java.util.Properties;

import javax.imageio.ImageIO;
import javax.imageio.ImageReader;
import javax.imageio.stream.ImageInputStream;
import javax.swing.ImageIcon;
import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.event.EventListenerList;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.htmlparser.*;
import org.htmlparser.filters.HasAttributeFilter;
import org.htmlparser.filters.StringFilter;
import org.htmlparser.filters.TagNameFilter;
import org.htmlparser.util.NodeList;
import org.htmlparser.util.ParserException;
import org.xml.*;

import processing.app.Base;
import processing.app.Sketchbook;

import antipasto.ScriptRunner;
import antipasto.GUI.ImageListView.ScriptCellRenderer;
import antipasto.Interfaces.*;
import antipasto.Util.Utils;

public class WingPanelAppStore extends JPanel {
	
	boolean isUpdated = false;
	String text;
	Font font;
	
	private Color bgDefaultColor =Color.WHITE;
	private Color bgHoverColor = Color.WHITE;
	private Color bgColor;
	
	private Color textDefaultColor = Color.BLACK;
	private Color textHoverColor = Color.WHITE;
	private Color textColor;
	
	private double width = 300; 
	private double height = 450;
	private boolean isFocused = false;
	private boolean isSelected = false;
	
	private JPanel     headerPanel;
	private JLabel     headerLabel;
	private JScrollPane scrollPane;
	private WingFooter  wingFooter;
	
	private JList 		appList;
	
	/**
	 * Creates a new Wing Panel for the App Store
	 * @param wingFooter The panel to display status to the user
	 * @param width The width of the panel
	 * @param height The height of the panel
	 */
	public WingPanelAppStore( Object wingFooter, 
							  int width, int height){

		this.wingFooter = (WingFooter)wingFooter;
		this.text = text;
		this.width = width;
		this.height = height;

		
		/* Setup the panel */
		setSize(width, height);
		setBackground(bgDefaultColor);
		setLayout(new BorderLayout());
		
		/* Build the header */
		headerPanel = initPanelHeader(" |Get App|  ");
		
		/* Build the body */
		initBody();
		
		/* Assemble the Panel */
		add(headerPanel, BorderLayout.NORTH);
		add(scrollPane, BorderLayout.CENTER);
	}
	
	/**
	 * Initialize a Panel Header
	 * @param textDisplay The button text
	 * @return: a JPanel with a button
	 */
	JPanel initPanelHeader(String textDisplay) {
		
		JPanel headerPanel = new JPanel();
		headerPanel.setBackground(new Color(0x04, 0x4F, 0x6F));
		headerPanel.setLayout(new BorderLayout());
		
		headerLabel = new JLabel(textDisplay);
		
		headerLabel.addMouseListener(new MouseListener() {

			public void mouseClicked(MouseEvent arg0) {
				// TODO Auto-generated method stub     
				
			}

			public void mouseEntered(MouseEvent arg0) {
			
				headerLabel.setForeground(Color.orange);
				
				wingFooter.setText(" Go Back ");
				
			}

			public void mouseExited(MouseEvent arg0) {
				headerLabel.setForeground(Color.white);
				wingFooter.setText(" ");
			}

			public void mousePressed(MouseEvent arg0) {
				
					wingFooter.setText("Ready.");
			}

			public void mouseReleased(MouseEvent arg0) {

			}
			
			});
		
		headerLabel.setForeground(new Color(0xFF, 0xFF, 0xFF));
		headerPanel.add(headerLabel, BorderLayout.CENTER);		
		
		return headerPanel;
	}
	
	/**
	 * Initialize the body of the panel.
	 */
	private void initBody() {
		
		
		appList = new JList(getApps());
		
		scrollPane = new JScrollPane(appList);
		scrollPane
				.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
		scrollPane
				.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);

		scrollPane.setVisible(true);
	}
	
	private void loadAppImages() {

		/**
		 * HTTP Image loading test code. 
		 */
		URL url = null;
		
		try {
			url = new URL("http://www.liquidware.com/icons/61/thumb/pingpong.jpg");
		} catch (MalformedURLException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		
		try {
			
			Object source; // File or InputStream
			InputStream inputStreamToURL = url.openStream(); 
			ImageInputStream iis = ImageIO.createImageInputStream(inputStreamToURL);

			Iterator readers = ImageIO.getImageReaders(iis);
			ImageReader read = (ImageReader) readers.next();
			
			read.setInput(iis,true,true); 
			Image image = read.read(0);
			
	        JLabel iconLabel = new JLabel();
			ImageIcon iconImg = new ImageIcon(image);
			iconLabel = new JLabel(iconImg);
			
			JLabel[] list = {iconLabel, iconLabel, iconLabel};
			
			//These are disabled until we create the model, collection, and CellRenderer
			//appList = new JList(list);
			//appList.setCellRenderer(new AppStoreCellRenderer());
			
			read.dispose(); 
			iis.close(); 
			inputStreamToURL.close();

			
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	/**
	 * 
	 * @return A string array of all the apps from the store
	 */
	private String[] getApps() {
		
	  String appURL = "http://www.liquidware.com/apps";
  	  //String proxy = "test.com";
	  //String port = "80";
      //Properties systemProperties = System.getProperties();
      //systemProperties.setProperty("http.proxyHost",proxy);
      //systemProperties.setProperty("http.proxyPort",port);
	
      Parser parser = null;
      NodeList appTitleNodes = null;
      NodeList appDescriptionNodes = null;
      NodeList appMoneyNodes = null;
      
      /**
       * Intentionally be quiet and return a blank list of 
       * Apps if we can't get the App listing for whatever reason.
       */
      String[] appTitleList = {" "};			
      							
      
      try {
 
		  parser = new Parser (appURL);
		  appTitleNodes = parser.parse(new HasAttributeFilter("class", "appproduct_title"));
		  
		  parser = new Parser (appURL);
	      appDescriptionNodes = parser.parse (new HasAttributeFilter("class", "appproduct_text"));
	      
		  parser = new Parser (appURL);
	      appMoneyNodes = parser.parse (new HasAttributeFilter("class", "money"));
	      
	      appTitleList = new String[appTitleNodes.size()];
	      
	      /* Store each App found */
	      for (int x=0; x< appTitleNodes.size(); x++) {
	    	  
	    	  String titleText = appTitleNodes.elementAt(x).getFirstChild().getText();
	    	  String descText = appDescriptionNodes.elementAt(x).getFirstChild().getText();
	    	  String moneyText = appMoneyNodes.elementAt(x).getFirstChild().getText();
	    	  
	    	  /* Simple String formatting for verbosely written Apps */
	    	  if (titleText.length() > 20) {
	    		  titleText = titleText.substring(0, 20);
	    	  }
	    	  
	    	  if (descText.length() > 30) {
	    		  descText = descText.substring(0, 30);
	    	  }
	    	  
	    	  /* Assemble the App listing */
	    	  appTitleList[x] = titleText + " - " +
	    	  					descText + " - " + 
	    	  					moneyText;
	      }
	      
		} catch (ParserException e) {
			
			 System.out.println ("App Store: Cannot load Apps listing");
		}

      return appTitleList;
		
	}
	
}
