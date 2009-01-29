package antipasto;

import java.io.File;
import java.io.FileFilter;
import java.util.ArrayList;
import java.util.List;

import antipasto.Interfaces.*;


public class ModuleLibrary {
	private IModule[] modules;
	
	public ModuleLibrary(String libraryFolderPath){
		File libDir = new File(libraryFolderPath);
		
		if(libDir.exists() && libDir.isDirectory()){
			this.loadModules(libDir);
		}else{
			libDir.mkdir();
		}
	}
	
	
	public IModule[] getGadgets(){
		return modules;
	}
	
	
	private void loadModules(File libDirectory){
		File[] files = libDirectory.listFiles();
		List list = new ArrayList();
		
		for(int i = 0; i < files.length; i++){
			if(files[i].getName().endsWith(Module.moduleExtension)){
				list.add(files[i]);
			}
		}
		IModule[] modules = new IModule[(list.size())];
		modules = (IModule[]) list.toArray();
		
		this.modules = modules;
	}
	
}
