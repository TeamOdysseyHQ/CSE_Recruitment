from setuptools import find_packages, setup
import os
from glob import glob
import glob as globmod

package_name = 'mini_model_description'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        (os.path.join('share', package_name, 'launch'), glob('launch/*')),
        (os.path.join('share', package_name, 'rviz'), glob('rviz/*')),
        (os.path.join('share', package_name, 'urdf'), glob('urdf/*')),
        (os.path.join('share', package_name, 'config'), glob('config/*')),
        *[(os.path.join('share', package_name, os.path.dirname(f)),
           [f]) for f in globmod.glob('meshes/**/*', recursive=True) if os.path.isfile(f)],  # meshes
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='Shaurya Pandey',
    maintainer_email='pashaurya2007@gmail.com',
    description='ROS2 package for rover model visualization using URDF and RViz2',
    license='Apache License 2.0',
    extras_require={
        'test': ['pytest'],
    },
    entry_points={
        'console_scripts': [],
    },
)
