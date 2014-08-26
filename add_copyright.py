# updates the copyright information for all .cs files
# usage: call recursive_traversal, with the following parameters
# parent directory, old copyright text content, new copyright text content

import os

excludedir = ["./dependencies", "./build_linux", "./build_mac", "./build_android", "./.git", "./gfx", "./level", "./cmake", "./cegui", "./language", "./materials", "./bin", "./VisualStudio2013", "./RTShaderLib", "./overlays", "./android/jni/includes"]
excludefile = ["./Zelda/Common/tinyxml2/tinyxml2.hpp", "./Zelda/Common/tinyxml2/tinyxml2.cpp", "./Zelda/Common/Physics/BtOgre.cpp", "./Zelda/Common/Physics/BtOgreExgtras.cpp", "./Zelda/Common/Physics/BtOgreExtras.hpp", "./Zelda/Common/Physics/BtOgreGP.hpp", "./Zelda/Common/Physics/BtOgrePG.hpp"]

def update_source(filename, oldcopyright, copyright):
    utfstr = chr(0xef)+chr(0xbb)+chr(0xbf)
    fdata = file(filename,"r+").read()
    isUTF = False
    if (fdata.startswith(utfstr)):
        isUTF = True
        fdata = fdata[3:]
    if (oldcopyright != None):
        if (fdata.startswith(oldcopyright)):
            fdata = fdata[len(oldcopyright):]
    if not (fdata.startswith(copyright)):
        print "updating "+filename
        fdata = copyright + fdata
        if (isUTF):
            file(filename,"w").write(utfstr+fdata)
        else:
            file(filename,"w").write(fdata)

def recursive_traversal(dir,  oldcopyright, copyright):
    global excludedir, excludefile
    fns = os.listdir(dir)
    print "listing "+dir
    for fn in fns:
        fullfn = os.path.join(dir,fn)
        if (fullfn in excludedir or fullfn in excludefile):
            continue
        if (os.path.isdir(fullfn)):
            recursive_traversal(fullfn, oldcopyright, copyright)
        else:
            if (fullfn.endswith(".cpp") or fullfn.endswith(".hpp")or fullfn.endswith(".java") or fullfn.endswith(".h")):
                update_source(fullfn, oldcopyright, copyright)


oldcright = file("COPYRIGHT","r+").read()
cright = file("COPYRIGHT","r+").read()
recursive_traversal(".", oldcright, cright)
exit()
