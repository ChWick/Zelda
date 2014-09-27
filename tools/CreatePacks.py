import os
import zipfile
import glob
import ntpath

def zipdir(path, zip):
    for root, dirs, files in os.walk(path):
        for file in files:
            zip.write(os.path.join(root, file))

def copyAllOfType(zipf, pattern, outputdir, recursive=False) :
    for f in glob.glob(pattern) :
        if f.endswith('~') :
            continue

        zipf.write(f, os.path.join(outputdir, ntpath.basename(f)), zipfile.ZIP_DEFLATED)
        
        if recursive :
            copyAllOfType(zipf, os.path.join(f, "*"), os.path.join(outputdir, ntpath.basename(f)), True)

                

def makeLightWorldZip() :
	print('Creating light_world.zip')

	zipf = zipfile.ZipFile('../packs/light_world.zip', 'w');

        # local resources
        copyAllOfType(zipf, '../packs/light_world/textures/*', 'textures')
        copyAllOfType(zipf, '../packs/light_world/materials/*', 'materials')
        copyAllOfType(zipf, '../packs/light_world/meshes/*', 'meshes')

        # other mesh fiels
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
        copyAllOfType(zipf, os.path.join(dataPath, 'language/*'), 'language', True)
	zipf.close()
	

# set this as working dir
os.chdir(os.path.dirname(os.path.realpath(__file__)))

if __name__ == '__main__':
    makeLightWorldZip()
    makeGameZip()
    makeSdkTrays()

    lightWorld = 'LightWorld'

    makeMapPack('link_house', lightWorld, ['physics_border_top.mesh', 'physics_floor.mesh', 'physics_floor_top.mesh', 'wall_bot_right.mesh', 'wall_bot.mesh', 'wall_to_water.mesh', 'house_red_roof.mesh', 'house_roof_border.mesh', 'house_wall.mesh', 'water.mesh'], includeHouse=True)
    makeMapPack('link_house_left', lightWorld, ['physics_floor.mesh', 'physics_floor_top.mesh', 'physics_floor_top_wall.mesh', 'physics_wall_bot.mesh'])
    makeMapPack('inner_house_link', lightWorld, ['physics_floor.mesh', 'physics_wall.mesh', 'vision_plane.mesh'])

