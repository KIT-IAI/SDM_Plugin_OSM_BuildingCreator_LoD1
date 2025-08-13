# OSM BuildingCreator LoD1

<img width="1380" height="351" alt="image" src="https://github.com/user-attachments/assets/17a65a2a-ae2e-4fd7-9f9c-70bc919af652" />

Generate CityGML LoD1 models ready for export from OpenStreetMap(OSM) data while preserving OSM attributes with the OSM BuildingCreator LoD1 plugin for the [KITModelViewer](https://github.com/KIT-IAI/SDM_KITModelViewer).

CityGML v2.0 documentation defines LoD1 as object blocks with flat roofs, while LoD2 includes differentiated roof structures and thematically differentiated boundary surfaces. In this case, the plugin generates LoD2 CityGML building models, but with flat roofs. The plugin is named after LoD1 because the LoD2 version, which includes roof structures and building parts, will be released soon.

## Step-by-step:

### 1- Load OSM Data by creating new map (a) or by opening OSM map (b).

<img width="254" height="169" alt="image" src="https://github.com/user-attachments/assets/929b312b-6b0d-4505-a52a-0b3098fd2147" />

#### a) Look for adress, connect to OSM servers and select OSM filter for buildings.

<img width="354" height="531" alt="image" src="https://github.com/user-attachments/assets/0339d70a-6261-4129-890c-d87a5934684a" />
<br><img width="789" height="361" alt="image" src="https://github.com/user-attachments/assets/1ceb3a93-8ccb-4eae-b8e2-5c2dfcfa5538" />

#### b) Open or drag OSM map data file retrieved from other source, such as [OpenStreetMap Export](https://www.openstreetmap.org/export). Models will be created just from buildings.

<img width="753" height="390" alt="image" src="https://github.com/user-attachments/assets/76223d9f-4b8a-4579-a4dc-638d5f05d792" />

<br><img width="627" height="327" alt="image" src="https://github.com/user-attachments/assets/c4a015dd-de02-4a36-8ed2-0a820c294374" />

### 2- Open plugin OSM Creator LoD1.

<img width="613" height="384" alt="image" src="https://github.com/user-attachments/assets/69adb870-9a30-4632-a277-2f5b7062c71f" />

<br>The Plugin offers two possibilities:
- **Create Boundary Surfaces and Building Solid**:
  Generate Solids with flat roofs of building footprints with given heights (LoD1) and boundary surfaces (Walls, roofs, ground) (LoD2).

  <img width="622" height="342" alt="image" src="https://github.com/user-attachments/assets/c5ad3e48-c6af-49bb-9161-0659b95dcc34" />


- **Create Building Solid only**:
  Geberate Solids with flat roofs of building footprints with given heights (LoD1).

  <img width="627" height="342" alt="image" src="https://github.com/user-attachments/assets/413e741d-eef9-4053-8640-d407d617573b" />

## Dependencies

No external dependencies

## How to cite

```bibtex
@software{SDM_Plugin_OSM_BuildingCreator_LoD1,
	title        = {SDM_Plugin_OSM_BuildingCreator_LoD1},
	author       = {{Fernanda Lourenzi, Andreas Geiger}},
	url          = {https://github.com/KIT-IAI/SDM_Plugin_OSM_BuildingCreator_LoD1},
	date         = {2025}
}
```




