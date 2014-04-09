camera [
  eye -4 3 3
  up 0 1 0
  focus 0 0 0
  angle 30
  ]

light [
  number 0
  color 0.8 0.8 0.8
  position 5 5 5
  ]

light [
  number 1
  color 0.5 0.5 0.5
  position -5 5 5
  ]

trans [
  translate 0 -0.6 0
  scale 6 0.2 6
  rotate 0 1 0 -90
  object cube [
    texture image/boccioni.bmp 1 1
    diffuse 0.5 0.5 0.5
    blend 0.5
    ]
  ]

trans [
  translate -0.8 0 -0.6
  rotate 0 1 1 45
  object sphere [
    texture image/flowers.bmp 3 3
    reflect 0.3 0.3 0.3
    diffuse 0 0.5 0
    blend 0.8
    shine 20
    specular 0 0 0
    transparent 0.7 0.7 0.7
    ]
  ]

trans [
  translate 0.8 0 -0.8
  object cube [
    reflect 0.1 0.1 0.1
    texture image/sun.bmp 1 1 
    blend 1.0
    ]
  ]

trans [
  translate 0.8 0 0.8
  scale 1.0 1.5 1.0
  object cylinder [
    reflect 0.9 0.9 0.9
    diffuse 0.8 0.8 0.8
    shine 30
    specular 1.0 1.0 1.0
    ]
  ]

trans [
  translate -0.8 0 1.0
  rotate 0 1 0 90
  scale 1.5 1.5 1.5
  object mesh [
    meshfile Mesh/cow.sm
    diffuse 0.6 0.1 0.0
    ]
  ]