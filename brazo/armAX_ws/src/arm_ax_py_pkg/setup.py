from setuptools import find_packages, setup

package_name = 'arm_ax_py_pkg'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='Jose Plascencia',
    maintainer_email='joseplascencia346@aragon.unam.mx',
    description='TODO: Package description',
    license='MIT',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'arm_ax_py_node = arm_ax_py_pkg.arm_ax_py_node:init_node'
        ],
    },
)
