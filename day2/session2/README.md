### Exercise 1. Create a simple geometry using GDML
In this exercise you will learn how to create a simple geometry using GDLM. Remember that this file has different sections:

**XML Declaration**

```
<?xml version="1.0" encoding="utf-8" ?>
```
**External files to be included** (in this case we are including material file)
```
<!DOCTYPE gdml [
<!ENTITY materials SYSTEM "https://rest-for-physics.github.io/materials/rest.xml">
]>
```
**XML Schema and GDML Schema location**

```
<gdml xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="http://service-spi.web.cern.ch/service-spi/app/releases/GDML/schema/gdml.xsd">
```
**Numerical definitions**
```
<define>
       <variable name="world_size" value="120" />
       
        <!-- The gas target -->
	      <variable name="targetGasDensity" value="1.66201e-3"/> <!-- in g/cm3 -->
        <variable name="gasTemperature" value="300"/> <!-- K -->
        <variable name="gasPressure" value="1"/> <!-- bar -->

        <!-- The vessel definitions -->  
        <constant name="vesselThickness" value="10" />    <!--in mm's -->
        <constant name="vesselXSize" value="100" />
	      <constant name="vesselYSize" value="100" />
        <constant name="vesselZSize" value="100"/>
        <position name="vesselPosition" unit="mm" x="0" y="0" z="0"/>
</define>
```
**Material definitions**
As we have included a file with material definitions, just need to referenciate it.
```
&materials;
```
**Geometry**

*Solid declarations*
```
<solids>
    <box name="WorldSolid" x="world_size" y="world_size" z="world_size" lunit="mm"/>
    <box name="solidBox" x="vesselXSize" y="vesselYSize" z="vesselZSize" lunit="mm" />
     <box name="gasSolid"  x="vesselXSize-2*vesselThickness" y="vesselYSize-2*vesselThickness" z="vesselZSize-2*vesselThickness" lunit="mm" />
    
    <subtraction name="vesselSolid">
        <first ref="solidBox"/>
        <second ref="gasSolid"/>
    </subtraction>      
</solids>
```
*Structure*
```
<structure>
    <!-- {{{ Volumes definition (material and solid assignment) -->
    <volume name="gasVolume">
        <materialref ref="PureArgon"/>
        <solidref ref="gasSolid"/>
    </volume>

    <volume name="vesselVolume">
        <materialref ref="G4_Cu"/>
        <solidref ref="vesselSolid"/>
    </volume>

    <!-- {{{ Physical volume definition (volume and position assignment) -->
     <volume name="World">
        <materialref ref="G4_AIR"/>
        <solidref ref="WorldSolid"/>

        <physvol name="gas">
            <volumeref ref="gasVolume"/>
            <position name="gasPosition"  x="0" y="0" z="0"/>
        </physvol>
       
        <physvol name="vessel">
            <volumeref ref="vesselVolume"/>
		<positionref ref="vesselPosition"/>
        </physvol>
    </volume>  

</structure>
```
**Definition of top volume**
```
<setup name="Default" version="1.0">
        <world ref="World"/>
    </setup>
```
**Final mark of GDM file**
```
</gdml>
```
**Activity 1:** Identify in the material repository (https://rest-for-physics.github.io/materials/rest.xml) the materials appearing in the GDML file. 
**Activity 2:** Create your own **box.gdml** file including all the sections and visualize the geometry using the REST macro  ```REST_Geant4_ViewGeometry("box.gdml")```

## Exercise 2. Create a simple RML file to launch simulations
