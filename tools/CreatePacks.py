import os
import zipfile
import glob
import ntpath

def zipdir(path, zip):
    for root, dirs, files in os.walk(path):
        for file in files:
            zip.write(os.path.join(root, file))

def copyAllOfType(zipf, pattern, outputdir) :
	for f in glob.glob(pattern) :
		zipf.write(f, os.path.join(outputdir, ntpath.basename(f)), zipfile.ZIP_DEFLATED)

def makeLightWorldZip() :
	print('Creating light_world.zip')

	zipf = zipfile.ZipFile('../packs/light_world.zip', 'w');

	zipf.write('../packs/light_world/textures/tiles.png', 'textures/tiles.png', zipfile.ZIP_DEFLATED)
	zipf.write('../packs/light_world/textures/house.png', 'textures/house.png', zipfile.ZIP_DEFLATED)
	zipf.write('../packs/light_world/materials/light_world.material', 'materials/light_world.material', zipfile.ZIP_DEFLATED)


	zipf.write('../maps/bush/GreenBush.mesh', 'meshes/GreenBush.mesh', zipfile.ZIP_DEFLATED)
	zipf.write('../maps/fence/fence_stake.mesh', 'meshes/fence_stake.mesh', zipfile.ZIP_DEFLATED)
	zipf.write('../maps/fence/fence_plank.mesh', 'meshes/fence_plank.mesh', zipfile.ZIP_DEFLATED)

	copyAllOfType(zipf, '../maps/stone/*.mesh', 'meshes')

	# tiles
	copyAllOfType(zipf, '../maps/water/*.mesh', 'meshes')
	copyAllOfType(zipf, '../maps/tiles/*.mesh', 'meshes')
	copyAllOfType(zipf, '../maps/tree/*.mesh', 'meshes')
	copyAllOfType(zipf, '../maps/flower/*.mesh', 'meshes')
	copyAllOfType(zipf, '../maps/flower/*.skeleton', 'skeletons')

	zipf.close()

def makeGameZip() :
	print('Creating game.zip')

	zipf = zipfile.ZipFile('../packs/game.zip', 'w');

	copyAllOfType(zipf, '../packs/game/textures/*', 'textures')
	copyAllOfType(zipf, '../packs/game/materials/*.material', 'materials')
	copyAllOfType(zipf, '../packs/game/programs/*', 'programs')
	

	copyAllOfType(zipf, '../models/link/*.mesh', 'meshes')
	copyAllOfType(zipf, '../models/link/*.skeleton', 'skeletons')
	copyAllOfType(zipf, '../models/link/link_*.png', 'textures')
	copyAllOfType(zipf, '../models/soldier/*.mesh', 'meshes')
	copyAllOfType(zipf, '../models/soldier/*.skeleton', 'skeletons')
	copyAllOfType(zipf, '../models/soldier/soldier_*.png', 'textures')
	copyAllOfType(zipf, '../models/Objects/rupee/*.mesh', 'meshes')
	zipf.write('../models/test/Cylinder.mesh', 'meshes/Cylinder.mesh', zipfile.ZIP_DEFLATED)

	zipf.close()

def makeSdkTrays() :
	print ("Creating SdkTrays.zip")

	zipf = zipfile.ZipFile('../packs/SdkTrays.zip', 'w')

	copyAllOfType(zipf, '../packs/SdkTrays/*', '.')

	zipf.close()

def makeMapPack(name, world, files, includeHouse=False) :
	print 'Creating map pack for', name, ' in ', world

	worldPath = os.path.join('../maps/Atlases', world)
	dataPath = os.path.join(worldPath, name)

	zipf = zipfile.ZipFile(os.path.join(worldPath, name + '.zip'), 'w')
	files.append(name + '.mesh')
	files.append(name + '.scene')
	files.append(name + '.xml')

	if (includeHouse) :
		files.append('house_window.mesh')
		files.append('house_roof_knob.mesh')
		files.append('house_entrance.mesh')
		files.append('house_knob.mesh')

	for file in files :
		zipf.write(os.path.join(dataPath, file), file, zipfile.ZIP_DEFLATED)
        # copy scripts
        copyAllOfType(zipf, os.path.join(dataPath, 'scripts/*'), 'scripts')
	zipf.close()
	

# set this as working dir
os.chdir(os.path.dirname(os.path.realpath(__file__)))

if __name__ == '__main__':
    makeLightWorldZip()
    makeGameZip()
    makeSdkTrays()

    makeMapPack('link_house', 'LightWorld', ['physics_border_top.mesh', 'physics_floor.mesh', 'physics_floor_top.mesh', 'wall_bot_right.mesh', 'wall_bot.mesh', 'wall_to_water.mesh', 'house_red_roof.mesh', 'house_roof_border.mesh', 'house_wall.mesh', 'water.mesh'], includeHouse=True)
    makeMapPack('link_house_left', 'LightWorld', ['physics_floor.mesh', 'physics_floor_top.mesh', 'physics_floor_top_wall.mesh', 'physics_wall_bot.mesh'])

