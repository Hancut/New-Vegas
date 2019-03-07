===============================================================================
__________________________  New Vegas - City Generator ________________________
===============================================================================

	For the beginning I created a matrix that represent the city where 0 means
builings/park and 1 means roads. The roads construction starts random on the map, 
and continue on different direction: left,  right or forward, but never behind.
2 constant are important in construction of the roads: maxTunnels and maxLenght.
maxTunnels represents the maximum number of lines(straight roads) on the map, and maxLenght
represents the maximum lenght of those lines. After setting the roads, I set the 
buildings and mini-parks(random). Are 2 types of buildings: one-unit building(occupies 1
unit in matrix) and four-units building(occupies 4 units in matrix). Mini-parks 
occupies 4 units in matrix and contains 4 trees, and some birds. Going through the matrix 
end if the neighbours(right, right-diagonal, down) are free (not roads) then it's 
a place for a mini-park or 4-units building.
	The buildings are procedural generate: are created with different geometric 
forms(cubes, hexagonal parallelepiped, cylinder) with different size of floors,
minimum 1 and maximum 4. 
	Every floor of the buildings are illuminate by one light which is positioned
above.
	The map is illuminated by a light, positioned above of the center of the city.

	Controls:
		- movement of the camera: * W - front
					  * A - left
					  * D - right
					  * S - back
					  * E - up
					  * Q - down
			* Rotate: right click + mouse