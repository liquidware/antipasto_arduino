package antipasto.Plugins;

import java.awt.*;
import javax.swing.*;
import javax.swing.event.*;

import java.awt.event.*;

import org.apache.log4j.Appender;
import org.apache.log4j.ConsoleAppender;
import org.apache.log4j.PropertyConfigurator;
import org.java.plugin.JpfException;
import org.apache.log4j.Logger;

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

import java.util.*;
import java.io.*;

import antipasto.Plugins.Manager.PluginListRenderer;


public class PluginLoader {

    public static final File pluginsDir = new File("plugins");
    private PluginManager pluginManager = null;
    private JPanel panel;
    private JFrame jf;
    private JScrollPane listpane;
    private JList list;
    private static final String LINE_SEP = System.getProperty("line.separator");
    private Logger logger;

    public PluginLoader() {
        prepareLoggers();
        initPluginManager();
        loadPlugins();
        startPlugins();
        showPluginPanel();
    }

    public PluginManager setManager(PluginManager pluginManager) {
        return this.pluginManager = pluginManager;
    }

    public PluginManager getManager() {
        return this.pluginManager;
    }
    public void prepareLoggers(){
        logger = Logger.getLogger(PluginLoader.class);
        LogOutputStream outStream = new LogOutputStream();
        PrintStream sysOut = new PrintStream(outStream, true);
        System.setOut(sysOut);
    }

    private void initPluginManager() {
        try {
            ExtendedProperties ep = new ExtendedProperties();
            ep.put("org.java.plugin.PathResolver", "org.java.plugin.standard.ShadingPathResolver");
            ep.put("unpackMode", "always");
            ep.put("org.java.plugin.standard.ShadingPathResolver.unpackMode", "always");
            pluginManager = ObjectFactory.newInstance(ep).createManager(
                    ObjectFactory.newInstance(ep).createRegistry(),
                    ObjectFactory.newInstance(ep).createPathResolver());

            setManager(pluginManager);

        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }

    private void loadPlugins() {
        try {
            File[] plugins = pluginsDir.listFiles(new FilenameFilter() {

                public boolean accept(File dir, String name) {
                    return name.toLowerCase().endsWith(".zip");
                }
            });

            PluginLocation[] locations = new PluginLocation[plugins.length];

            for (int i = 0; i < plugins.length; i++) {
                locations[i] = StandardPluginLocation.create(plugins[i]);
                System.out.println(plugins[i] + " loaded");
            }

            System.out.println(plugins.length + " plugins loaded");

            getManager().publishPlugins(locations);

        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }

    void startPlugins() {
        try {
            PluginDescriptor core = getManager().getRegistry().getPluginDescriptor("com.plugin.core");
            ExtensionPoint point = getManager().getRegistry().getExtensionPoint(core.getId(), "Section");

            for (Iterator it = point.getConnectedExtensions().iterator(); it.hasNext();) {
                Extension ext = (Extension) it.next();
                System.out.println("activating: " + ext);
                PluginDescriptor descr = ext.getDeclaringPluginDescriptor();
                getManager().activatePlugin(descr.getId());
            }

        } catch (PluginLifecycleException e) {
            System.out.println("ERROR starting plugin: " + e.getMessage());
        }

    }

    void showPluginPanel() {

        jf = new JFrame("PluginLoader - Manager");
        jf.setSize(300, 300);
        jf.setLayout(new BorderLayout());

        JButton reload_btn = new JButton("reload list");
        ActionListener actionListener = new ActionListener() {

            public void actionPerformed(ActionEvent actionEvent) {
                updatePlugins();
            }
        };
        reload_btn.addActionListener(actionListener);


        jf.add(reload_btn, BorderLayout.PAGE_START);

        jf.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);

        listpane = new JScrollPane();
        //listpane.setLayout(new FlowLayout());
        listpane.setSize(300, 150);
        listpane.setBorder(BorderFactory.createEmptyBorder(0, 10, 10, 10));


        jf.add(listpane, BorderLayout.CENTER);

        updatePlugins();

        panel = new JPanel();

        panel.setLayout(new BoxLayout(panel, BoxLayout.LINE_AXIS));
        panel.setBorder(BorderFactory.createEmptyBorder(0, 10, 10, 10));
        panel.add(Box.createHorizontalGlue());
        panel.add(Box.createRigidArea(new Dimension(10, 0)));
        panel.setSize(300, 50);


        //panel.setVisible(false);

        jf.add(panel, BorderLayout.PAGE_END);
        jf.setVisible(true);
        listpane.revalidate();

    }

    private void updatePlugins() {

        list = new JList();
        PluginListRenderer renderer = new PluginListRenderer();
        list.setCellRenderer(renderer);     
        list.setSize(300, 200);

        //int cellWidth = 300;
        //list.setFixedCellWidth(cellWidth);
        int cellHeight = 28;
        list.setFixedCellHeight(cellHeight);

        loadPlugins();
        final PluginListModel listModel = listRegisteredPlugins();
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
        Boolean isactive = getManager().isPluginActivated(descr);
        Boolean isenabled = getManager().isPluginEnabled(descr);
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
                        getManager().disablePlugin(pdescr);
                        //getManager().enablePlugin(pdescr);
                        updatePlugins();
                    }
                });

        ActionListener al;

        if (isactive) {
            al = new ActionListener() {

                public final void actionPerformed(final ActionEvent event) {
                    getManager().deactivatePlugin(id);
                    updatePlugins();
                }
            };
        } else {
            al = new ActionListener() {

                public final void actionPerformed(final ActionEvent event) {
                    try {
                        getManager().activatePlugin(id);
                        updatePlugins();
                    } catch (Exception e) {
                    }
                }
            };
        }

        startstop_btn.addActionListener(al);


        panel.setVisible(true);
    }

    PluginListModel listRegisteredPlugins() {
        PluginListModel listModel = new PluginListModel();
        try {
            PluginDescriptor core = getManager().getRegistry().getPluginDescriptor("com.plugin.core");
            ExtensionPoint point = getManager().getRegistry().getExtensionPoint(core.getId(), "Section");

            for (Iterator it = point.getConnectedExtensions().iterator(); it.hasNext();) {
                Extension ext = (Extension) it.next();
                System.out.println("listing: " + ext);
                PluginDescriptor descr = ext.getDeclaringPluginDescriptor();


                //ClassLoader classLoader = getManager().getPluginClassLoader(descr);
                //Class<?> pclass= classLoader.loadClass(ext.getParameter("class").valueAsString());
                //Plugin psec = (Plugin) pclass.newInstance();


                listModel.addElement(descr, ext.getParameter("class").valueAsString());
            }

        } catch (Exception e) {
            System.out.println("ERROR listing plugins: " + e.getMessage());
        }
        return listModel;
    }
    /**************

    PluginDescriptor[] plugins = new PluginDescriptor[extensions.size()];

    final String jpfPluginID = ext.getDeclaringPluginDescriptor().getId();
    final String pluginID = "12"
    final BeenPluginDescriptor desc = new PluginDescriptor(pluginID);
    desc.setName(getPluginName(pluginID));
    if (ext.getDeclaringPluginDescriptor().getDocumentation() != null) {
    desc.setDescription(ext.getDeclaringPluginDescriptor().getDocumentation().getText());
    }
    String pluginVersion = ext.getDeclaringPluginDescriptor().getVersion().toString();
    if (! StringUtils.isEmpty(pluginVersion)) {
    desc.setVersion(pluginVersion);
    }

    plugins[i++] = desc;

    DefaultPluginsCollector collector = new DefaultPluginsCollector();
    try {
    collector.configure(bootProperties);
    } catch (Exception e) {
    throw new ComponentInitializationException(e);
    }
    Collection locations = collector.collectPluginLocations();
    List<PluginLocation> newLocations = new ArrayList<PluginLocation>();


     ****************/

     class LogOutputStream extends ByteArrayOutputStream {

        LogOutputStream() {
            super();
        }

        public void flush() throws IOException {
            String message = toString();
            if (!LINE_SEP.equals(message) && !"".equals(message)) {
                logger.debug(message);
            }
            reset();
        }
    }
}