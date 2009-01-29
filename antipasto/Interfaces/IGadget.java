package antipasto.Interfaces;



public interface IGadget extends IVersionable, IConfigurable {
    public void setModule(IModule[] gadgets);
    public IModule[] getModules();
    public String getName();
    public void setName(String name);

    public void beginBuildGadgets();
    public IModule getNextGadget();
}
