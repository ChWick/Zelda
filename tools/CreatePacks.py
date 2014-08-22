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
	zipf.write('../maps/fence/fence_stake.mesh', 'meshes/fence_stake.mesh', zipfile.ZIP_DEFLATED)
	zipf.write('../maps/fence/fence_plank.mesh', 'meshes/fence_plank.mesh', zipfile.ZIP_DEFLATED)

	zipf.close()

def makeGameZip() :
	print('Creating game.zip')

	zipf = zipfile.ZipFile('../packs/game.zip', 'w');

	zipf.write('../packs/game/materials/rupee.material', 'materials/rupee.material', zipfile.ZIP_DEFLATED)
	zipf.write('../models/test/Cylinder.mesh', 'meshes/Cylinder.mesh', zipfile.ZIP_DEFLATED)

	zipf.close()

def makeLWLinkHouseZip() :
	print('Creating link_house.zip')

	zipf = zipfile.ZipFile('../maps/Atlases/LightWorld/link_house.zip', 'w')
	zipf.write('../maps/Atlases/LightWorld/link_house/physics_border_top.mesh', 'physics_border_top.mesh', zipfile.ZIP_DEFLATED)
	zipf.write('../maps/Atlases/LightWorld/link_house/physics_floor.mesh', 'physics_floor.mesh', zipfile.ZIP_DEFLATED)
	zipf.write('../maps/Atlases/LightWorld/link_house/physics_floor_top.mesh', 'physics_floor_top.mesh', zipfile.ZIP_DEFLATED)
	zipf.write('../maps/Atlases/LightWorld/link_house/wall_bot_right.mesh', 'wall_bot_right.mesh', zipfile.ZIP_DEFLATED)
	zipf.write('../maps/Atlases/LightWorld/link_house/wall_bot.mesh', 'wall_bot.mesh', zipfile.ZIP_DEFLATED)
	zipf.write('../maps/Atlases/LightWorld/link_house/wall_to_water.mesh', 'wall_to_water.mesh', zipfile.ZIP_DEFLATED)
	zipf.write('../maps/Atlases/LightWorld/link_house/link_house.mesh', 'link_house.mesh', zipfile.ZIP_DEFLATED)
	zipf.write('../maps/Atlases/LightWorld/link_house/link_house.scene', 'link_house.scene', zipfile.ZIP_DEFLATED)
	zipf.write('../maps/Atlases/LightWorld/link_house/link_house.xml', 'link_house.xml', zipfile.ZIP_DEFLATED)

	zipf.close()

if __name__ == '__main__':
    makeLightWorldZip()
    makeGameZip()
    makeLWLinkHouseZip()