
# Why should i audit plugins?

UE4 is an enormous code base. Plugins are add-ons which extend the functionality available to the user. They're great!
However, there are also a lot of them. Many you will never use, are 'Enabled By Default'

### Each plugin has a footprint. 

Plugins may have modules. These modules have source files. If a plugin is enabled, it's modules may be built for your project. Each can add dozens/scores/hundreds of compile actions to your builds, depending on complexity. These actions may also add to your executable size in your final project.
Plugins may also have content. This content can be in the form of lots of Assets. These Assets may be being cooked, and added to your package. Even though you may never use them. If they make it to your package, these may heavily increase the size of your pak file for installed projects. It will also have an impact on the cook time of your project.

# What does this tool do?

The Detective Plugin extension will allow you, the user, to quickly discover which plugins may be enabled for your project. If you see any you don't need, you can disable these directly from the Tool.

The tool also shows which plugins are enabled for which platforms. This should allow you to make decisions based on whether you need these plugins enabled for your platform.

