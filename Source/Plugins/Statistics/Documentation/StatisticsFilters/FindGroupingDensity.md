Find Grouping Densities {#findgroupingdensities}
=============

## Group (Subgroup) ##

Statistics (Reconstruction)

## Description ##

This **Filter** calculates the grouping densities for specific **Parent Features**.  This filter is intended to be used for hierarchical reconstructions (i.e., reconstructions involving more than one segmentation; thus, the **Feature**-**Parent Feature** relationship). The **Filter** iterates through all **Features** that belong to each **Parent Feature,** querying each of the **Feature** **Neighbors** to determine if it was checked during grouping. A list of **Checked Features** is kept for each **Parent Feature**.  Then, each **Parent Volume** is divided by the corresponding total volume of **Checked Features** to give the **Grouping Densities**.

If non-contiguous neighbors were used in addition to standard neighbors for grouping, then the *Use Non-Contiguous Neighbors* Parameter may be used.

Since many **Checked Features** are checked by more than one **Feature** during grouping, a premium is placed on the **Parent Feature** querying the **Checked Feature** having the largest **Parent Volume.**  For **Checked Features** to be written, the *Find Checked Features* Parameter may be used.


## Parameters ##

| Name | Type | Description |
|------|------| ----------- |
| Use Non-Contiguous Neighbors | bool | Whether to also use **Neighborhoods** in addition to **Neighbors** in the **Grouping Densities** calculation. |
| Find Checked Features | bool | Whether to write **Checked Features**. |

## Required Geometry ##

Cell

## Required Objects ##

| Kind | Default Name | Type | Component Dimensions | Description |
|------|--------------|------|----------------------|-------------|
| **Attribute Matrix** | FeatureData | Feature | N/A | **Feature Attribute Matrix** of the selected _Feature Ids_ |
| **Feature Attribute Array** | Volumes | float | (1) | Volume of the **Feature** |
| **Feature Attribute Array** | ParentIds | float | (1) | _Parent Id_ of the **Feature** |
| **Feature Attribute Array**  | NeighborList | List of int32_t | (1) | List of the contiguous neighboring **Features** for a given **Feature** |
| **Feature Attribute Array** | NeighborhoodLists | List of int32_t | (1) | List of the **Features** whose centroids are within the user specified multiple of equivalent sphere diameter from each **Feature** |
| **Attribute Matrix** | FeatureData | Feature | N/A | **Parent Attribute Matrix** of the selected _Parent Ids_ |
| **Feature Attribute Array** | ParentVolumes | float | (1) | Volume of the **Parent Feature** |


## Created Objects ##

| Kind | Default Name | Type | Component Dimensions | Description |
|------|--------------|------|----------------------|-------------|
| **Feature Attribute Array** | CheckedFeatures |  int32_t | (1) | The **Features** that were checked during grouping for a given **Parent Feature**.  This is written as the **ParentId** with the largest *Parent Volume* of the **Parent Features** that checked it. |
| **Feature Attribute Array** | GroupingDensities | float | (1) | **Parent Volume** divided by the sum of the **Checked Features** for the **Parent Feature** |

## License & Copyright ##

Please see the description file distributed with this **Plugin**

## DREAM.3D Mailing Lists ##

If you need more help with a **Filter**, please consider asking your question on the [DREAM.3D Users Google group!](https://groups.google.com/forum/?hl=en#!forum/dream3d-users)


