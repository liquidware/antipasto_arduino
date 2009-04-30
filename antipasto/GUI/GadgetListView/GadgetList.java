package antipasto.GUI.GadgetListView;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.color.ColorSpace;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.geom.Rectangle2D;
import java.awt.geom.RoundRectangle2D;
import java.io.*;

import javax.swing.*;
import javax.swing.event.EventListenerList;

import antipasto.GadgetFactory;
import antipasto.ModuleFactory;
import antipasto.GUI.GadgetListView.GadgetPanelEvents.IActiveGadgetChangedEventListener;
import antipasto.GUI.GadgetListView.GadgetPanelEvents.IActiveSketchChangingListener;
import antipasto.GUI.GadgetListView.GadgetPanelEvents.SketchChangingObject;
import antipasto.Interfaces.IGadget;
import antipasto.Interfaces.IModule;
import antipasto.Interfaces.IPackedFile;
import antipasto.Interfaces.ITemporary;


public class GadgetList extends JList implements IGadgetWindowTransferFileEventListener/*, MouseListener*/{

    IGadget gadget;
    JList jList;
    GadgetCollection _collection;
    private String moduleDirectory;
    private boolean mouseIsOver = false;
    private Rectangle bounding;
    
    

    private EventListenerList activeGadgetChangedEventList = new EventListenerList();
    private EventListenerList activeSketchChangedEventList = new EventListenerList();

    public GadgetList(IGadget gadget, String gadgetDirectory){
        super();
        this.loadGadget(gadget);
        this.moduleDirectory = gadgetDirectory;
        init();
    }

    public GadgetList(String gajDirectory){
        super();
        this.moduleDirectory = gajDirectory;
        init();
    }

    public void loadGadget(IGadget gaj){
        this.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        gadget = gaj;
        _collection = new GadgetCollection(gaj);
        
        this.setModel(new GadgetListModel(_collection));
        this.setCellRenderer(new GadgetCellRenderer());
        this.setSelectedIndex(0);
        this.setDragEnabled(false);
    }
    
    public void addGadgetToCurrentSketchBook(File f) {
        if(f.getName().endsWith(".module")){
            ModuleFactory fact = new ModuleFactory();
            try {
                this.loadGadget(new GadgetFactory().AddModuleToGadget(gadget, fact.loadModule(f, System.getProperty("java.io.tmpdir"), true)));
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public void loadGadget(File f) {
        if(f.getName().endsWith(".pde")){
            this.fireSketchChangingEvent(this.gadget, f);
        }
    }

    public void addActiveGadgetChangeEventListener(IActiveGadgetChangedEventListener listener) {
        this.activeGadgetChangedEventList.add(IActiveGadgetChangedEventListener.class, listener);
    }

    public void removeActiveGadgetChangeEventListener(IActiveGadgetChangedEventListener listener) {
        this.activeGadgetChangedEventList.remove(IActiveGadgetChangedEventListener.class, listener);
    }

    public void onFileTransfer(FileTransferObject ftObj) {
        File f = ftObj.getFile();

        if (f.getName().endsWith(IModule.moduleExtension)) {
            this.addGadgetToCurrentSketchBook(f);
        } else if (f.getName().endsWith(".pde") || f.getName().endsWith(".gadget")){
            this.loadGadget(f);
        }
    }

    public void closeCurrentSketch(){
        this.saveCurrentGadget();
    }

    public void saveCurrentGadget(){
        GadgetFactory factory = new GadgetFactory();
        //factory.writeSketchBookToFile(_book, sketchBookDirectory);
		String originalDirectory = ((IPackedFile)gadget).getPackedFile().getPath();
		String bookFileName = ((IPackedFile)gadget).getPackedFile().getName();
		System.out.println("Saving gadget called : " + bookFileName);
		antipasto.Interfaces.ITemporary temp = (antipasto.Interfaces.ITemporary)gadget;
		IModule[] newGadgets = new IModule[gadget.getModules().length];
		for(int i = 0; i < gadget.getModules().length; i++){
		    IModule curGadget = gadget.getModules()[i];
		    if(curGadget instanceof IPackedFile && curGadget instanceof ITemporary){
		        File gadgetDirectory = new File(((ITemporary)curGadget).getTempDirectory());
		        File packedFile = ((IPackedFile)curGadget).getPackedFile();

		        ModuleFactory fact = new ModuleFactory();
		        try{
		        	fact.WriteModuleToFile(curGadget, packedFile.getPath().substring(0,
		        			packedFile.getPath().length() - packedFile.getName().length()));
		        }catch(Exception ex){
		            ex.printStackTrace();
		        }
		        newGadgets[i] = curGadget;
		    }
		}
		gadget.setModule(newGadgets);
		File tempDirectory = new File(temp.getTempDirectory());
		int endIndex = originalDirectory.length() - bookFileName.length();
		originalDirectory = originalDirectory.substring(0, endIndex);
		factory.writeGadgetToFile(gadget, ((IPackedFile)gadget).getPackedFile());
    }

    public String getSketchBookDirectory() {
        return moduleDirectory;
    }

    public void setSketchBookDirectory(String sketchBookDirectory) {
        this.moduleDirectory = sketchBookDirectory;
    }

    public void addSketchChangingeListener(IActiveSketchChangingListener listener){
        this.activeSketchChangedEventList.add(IActiveSketchChangingListener.class, listener);
    }

    public void removeSketchChangingListener(IActiveSketchChangingListener listener){
        this.activeSketchChangedEventList.remove(IActiveSketchChangingListener.class, listener);
    }

    private void fireSketchChangingEvent(IGadget oldSketch, File newFile){
        Object[] listeners = activeSketchChangedEventList.getListenerList();
                    // Each listener occupies two elements - the first is the listener class
                    // and the second is the listener instance
                    for (int i=0; i<listeners.length; i+=2) {
                        if (listeners[i]== IActiveSketchChangingListener.class) {
                            ((IActiveSketchChangingListener)listeners[i+1]).onActiveSketchChanged(new SketchChangingObject(oldSketch, newFile));
                        }
                    }
    }

	public void doImportDragDrop(IModule module){
		//TODO: Do this later!
		//System.out.println("Drag drop not implemented just yet.....I'm only on programmer!");
		System.out.println("Attempting import");
		if(module instanceof IPackedFile){
			IPackedFile file = (IPackedFile)module;
			String name = module.getName();
			int x = 0;
			while(this.checkName(this.gadget, name)){
				x++;
				name = name + "(" + x +")";
			}
			File dest = new File(this.getSketchBookDirectory() + File.separator + name + IModule.moduleExtension);
			try {
				GadgetList.copyFile(file.getPackedFile(), dest);
			} catch (IOException e) {
				e.printStackTrace();
			}
			IModule[] modules = this.gadget.getModules();
			IModule[] modules2 = new IModule[modules.length + 1];
			int i = 0;
			for(; i < modules.length; i++){
				modules2[i] = modules[i];
			}
			ITemporary tempFile = (ITemporary)(module);
			ModuleFactory fact = new ModuleFactory();
			String directory = tempFile.getTempDirectory();
			try {
				modules2[i] = fact.loadModule(dest, directory, false);
			} catch (Exception e) {
				e.printStackTrace();
			}
			GadgetFactory gadgetFactory = new GadgetFactory();
			try {
				gadgetFactory.AddModuleToGadget(gadget, modules2[i]);
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			this.saveCurrentGadget();
			this.loadGadget(gadget);
		}else{
			System.out.println("Error trying to import the module...");
		}
	}
	
	private boolean checkName(IGadget gadget, String name){
		IModule[] modules = gadget.getModules();
		int ret = 0;
		for(int i = 0; i < modules.length; i++){
			if(name.equalsIgnoreCase(modules[i].getName()))
				return true;
		}
		return false;
	}
	
	
	private void init(){
		final GadgetList gList = this;
		this.addMouseMotionListener(new MouseMotionListener(){
			public void mouseMoved(MouseEvent e) {
				System.out.println("mouse moved");
				Point p = new Point(e.getX(),e.getY());
				int index = gList.locationToIndex(p);
				if(index >= 0){
					System.out.println("Mouse is over = " + true);
					gList.bounding = (Rectangle)gList.getCellBounds(index, index);
					gList.mouseIsOver = true;
					gList.repaint();
				}else{
					System.out.println("Mouse is over = " + false);
					gList.mouseIsOver = false;
					gList.repaint();
				}
			}

			public void mouseDragged(MouseEvent e) {
			}
			
		});
		this.addMouseListener(new MouseListener(){

			public void mouseClicked(MouseEvent e) {
			}

			public void mouseEntered(MouseEvent e) {
			}

			public void mouseExited(MouseEvent e) {
				mouseIsOver = false;
				repaint();
			}

			public void mousePressed(MouseEvent e) {
			}

			public void mouseReleased(MouseEvent e) {
			}
			
		});
	}
	
	public void paint(Graphics g){
		super.paint(g);
		Graphics2D g2d = (Graphics2D)g;
		if(this.mouseIsOver){
			System.out.println("Drawing....");
			RoundRectangle2D outer = new RoundRectangle2D.Double((double)this.bounding.getX(), (double)this.bounding.getY(), (double)this.bounding.getWidth(), (double)this.bounding.getHeight(), 10, 10);
			Rectangle2D inner = new Rectangle2D.Double((double)this.bounding.getX() + 5, (double)this.bounding.getY() + 5, (double)this.bounding.getWidth()- 10, (double)this.bounding.getHeight() - 10);//, 10, 10);
			g2d.setColor(Color.gray);
			g2d.draw(outer);
		}
	}
	
	//This code was copied and pasted directly from
	//http://www.codeandcoffee.com/2006/08/22/quick-snippets-copy-a-file-with-java/
	public static void copyFile(File fSource, File fDest) throws IOException
	{
		// Declare variables
		InputStream sIn = null;
		OutputStream sOut = null;
	
		try
		{
		// Declare variables
		int nLen = 0;
		sIn = new FileInputStream(fSource);
		sOut = new FileOutputStream(fDest);
	
		// Transfer bytes from in to out
		byte[] bBuffer = new byte[1024];
		while ((nLen = sIn.read(bBuffer)) > 0)
		{
		sOut.write(bBuffer, 0, nLen);
		}
	
		// Flush
		sOut.flush();
		}
		finally
		{
		// Close streams
		try
		{
		if (sIn != null)
		sIn.close();
		if (sOut != null)
		sOut.close();
		}
		catch (IOException eError)
		{
		}
		}
	}
}
