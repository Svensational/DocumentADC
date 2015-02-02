#DocumentADC

The inspiration for this programm was an algorithm I learned in a computer vision lecture in the university. It's goal was to remove the background of written documents, mostly arising from shading. This is achieved by first removing the dark thin lines from text with a max filter to obtain an image of the background which can then be removed from the original image. 

This simple algorithm works pretty well and can be optimized easily from O(n^2) to O(c) per pixel, rendering the kernel size irrelevant. 

For now only grayscale images can be processed and the GUI lacks posibilities to alter the settings, but the algorithms work pretty fast and scale with the amount of available CPU cores. The results aren't shown in the GUI neither but can be saved as Images or as a single multipage PDF file. 
