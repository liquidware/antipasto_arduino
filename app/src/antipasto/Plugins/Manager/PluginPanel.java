package antipasto.Plugins.Manager;

import java.awt.*;
import javax.swing.*;
import javax.swing.event.*;
import java.awt.event.*;

import org.java.plugin.ObjectFactory;
import org.java.plugin.PluginLifecycleException;
import org.java.plugin.PluginManager;
import org.java.plugin.PluginManager.PluginLocation;
import org.java.plugin.standard.StandardPluginLocation;
import org.java.plugin.registry.Extension;
import org.java.plugin.registry.ExtensionPoint;
import org.java.plugin.registry.PluginDescriptor;
import org.java.plugin.util.ExtendedProperties;
import org.java.plugin.PluginClassLoader;
import org.java.plugin.boot.DefaultPluginsCollector;
import org.java.plugin.registry.IntegrityCheckReport;
import org.java.plugin.registry.ManifestProcessingException;
import org.java.plugin.registry.PluginAttribute;
import org.java.plugin.registry.PluginRegistry;
import org.java.plugin.registry.IntegrityCheckReport.ReportItem;
import org.java.plugin.util.IoUtil;

import antipasto.Plugins.PluginLoader;
import antipasto.Plugins.Manager.PluginListModel;

public class PluginPanel extends JFrame{
    
	PluginLoader loader;
	private JPanel panel;
	private JFrame jf;
	private JScrollPane listpane;
	private JList list;
	    
    public PluginPanel(PluginLoader l){
        setSize(300, 300);
        setLayout(new BorderLayout());

        loader = l;
        
        buildPluginPanel();
        hidePluginPanel();
    }
    
    private void buildPluginPanel() {

        JButton reload_btn = new JButton("reload");
        ActionListener actionListener = new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
            	updatePlugins();
            }
        };
        reload_btn.addActionListener(actionListener);


        add(reload_btn, BorderLayout.PAGE_START);

        setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);

        
        
        listpane = new JScrollPane();
        listpane.setSize(300, 150);
        listpane.setBorder(BorderFactory.createEmptyBorder(0, 10, 10, 10));
        add(listpane, BorderLayout.CENTER);

        panel = new JPanel();

        panel.setLayout(new BoxLayout(panel, BoxLayout.LINE_AXIS));
        panel.setBorder(BorderFactory.createEmptyBorder(0, 10, 10, 10));
        panel.add(Box.createHorizontalGlue());
        panel.add(Box.createRigidArea(new Dimension(10, 0)));
        panel.setSize(300, 50);
        panel.setVisible(false);
        add(panel, BorderLayout.PAGE_END);

        
    }

    /**
     * Shows the plugin panel
     * 
     */
    public void showPluginPanel() {

        setVisible(true);

    }
    
    /**
     * Hides the plugin panel
     * 
     */
    public void hidePluginPanel() {

        setVisible(false);

    }
        
    private void updatePlugins() {
    	
    	loader.loadPlugins();
        
        list = new JList();
        PluginListRenderer renderer = new PluginListRenderer();
        list.setCellRenderer(renderer);     
        list.setSize(300, 200);

        //int cellWidth = 300;
        //list.setFixedCellWidth(cellWidth);
        int cellHeight = 28;
        list.setFixedCellHeight(cellHeight);

 
        final PluginListModel listModel = loader.listRegisteredPlugins();
        list.setModel(listModel);
        list.updateUI();
        ListSelectionModel listSelectionModel = list.getSelectionModel();
        listSelectionModel.addListSelectionListener(new ListSelectionListener() {

            public void valueChanged(ListSelectionEvent e) {
                if (e.getValueIsAdjusting() == false) {
                    if (list.getSelectedIndex() == -1) {
                    } else {
                        int idx = list.getSelectedIndex();
                        selectPlugin(listModel.getDescriptorAt(idx));
                    }
                }
            }
        });
        listpane.removeAll();
        listpane.updateUI();
        listpane.add(list);
    }

    private void selectPlugin(PluginDescriptor descr) {
        System.out.println(descr.getLibraries());
        panel.removeAll();
        panel.updateUI();
        Boolean isactive = loader.getManager().isPluginActivated(descr);
        Boolean isenabled = loader.getManager().isPluginEnabled(descr);
        final String id = descr.getId();
        final PluginDescriptor pdescr = descr;
        System.out.println("SELECTED: " + descr);
        System.out.println("  active: " + isactive);
        System.out.println("  isloaded: " + isenabled);

        JTextField textField = new JTextField(20);
        JButton enabledisable_btn = new JButton("enable plugin");
        JButton startstop_btn = new JButton("start plugin");

        panel.add(textField);
        panel.add(Box.createRigidArea(new Dimension(10, 0)));
        panel.add(enabledisable_btn);
        panel.add(Box.createRigidArea(new Dimension(10, 0)));
        panel.add(startstop_btn);

        textField.setText(descr.toString());
        enabledisable_btn.setLabel(!isenabled ? "Enable" : "Disable");
        startstop_btn.setLabel(!isactive ? "Activate" : "Deactivate");

        enabledisable_btn.addActionListener(
                new ActionListener() {

                    public final void actionPerformed(final ActionEvent event) {
                    	loader.getManager().disablePlugin(pdescr);
                        //getManager().enablePlugin(pdescr);
                        updatePlugins();
                    }
                });

        ActionListener al;

        if (isactive) {
            al = new ActionListener() {

                public final void actionPerformed(final ActionEvent event) {
                	loader.getManager().deactivatePlugin(id);
                    updatePlugins();
                }
            };
        } else {
            al = new ActionListener() {

                public final void actionPerformed(final ActionEvent event) {
                    try {
                    	loader.getManager().activatePlugin(id);
                        updatePlugins();
                    } catch (Exception e) {
                    }
                }
            };
        }

        startstop_btn.addActionListener(al);

        panel.setVisible(true);
    }
}
