import os
import zipfile

def zipdir(path, zip):
    for root, dirs, files in os.walk(path):
        for file in files:
            zip.write(os.path.join(root, file))

def makeLightWorldZip() :
	print('Creating light_world.zip')

	zipf = zipfile.ZipFile('../packs/light_world.zip', 'w');

	zipf.write('../packs/light_world/textures/tiles.png', 'textures/tiles.png', zipfile.ZIP_DEFLATED)
	zipf.write('../packs/light_world/textures/house.png', 'textures/house.png', zipfile.ZIP_DEFLATED)
	zipf.write('../packs/light_world/materials/light_world.material', 'materials/light_world.material', zipfile.ZIP_DEFLATED)


	zipf.write('../maps/bush/GreenBush.mesh', 'meshes/GreenBush.mesh', zipfile.ZIP_DEFLATED)

	zipf.close()

def makeGameZip() :
	print('Creating game.zip')

	zipf = zipfile.ZipFile('../packs/game.zip', 'w');

	zipf.write('../packs/game/materials/rupee.material', 'materials/rupee.material', zipfile.ZIP_DEFLATED)
	zipf.write('../models/test/Cylinder.mesh', 'meshes/Cylinder.mesh', zipfile.ZIP_DEFLATED)

	zipf.close()

if __name__ == '__main__':
    makeLightWorldZip()
    makeGameZip()