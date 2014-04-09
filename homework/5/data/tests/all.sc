camera [
  eye -4 3 3
  up 0 1 0
  focus 0 0 0
  angle 30
  ]

light [
  number 0
  color 0.3 0.3 0.3
  position 10 10 10
  ]

light [
  number 1
  color 0.1 0.1 0.1
  position 0 0 0
  ]

trans [
  translate 0 -0.6 0
  scale 6 0.2 6
  object cube [
    texture C:\Users\Remco\Desktop\RayTracer\image\earth.bmp 1 1
    diffuse 1 1 0.5
    blend 0.8
    ]
  ]

trans [
  translate -0.8 0 -0.8
  rotate 0 1 1 45
  object sphere [
    texture image\flowers.bmp 3 3
    reflect 0.3 0.3 0.3
    diffuse 0 1 0
    blend 0.8
    shine 20
    specular 0 0 0
    ]
  ]

trans [
  translate 0.8 0 -0.8
  object cube [
    reflect 0.5 0.5 0.5
    texture image/sun.bmp 1 1 
    blend 1.0
    ]
  ]

trans [
  translate 0.8 0 0.8
  object cylinder [
    reflect 0.9 0.9 0.9
    diffuse 1.0 1.0 1.0
    shine 30
    specular 1.0 1.0 1.0
    ]
  ]

trans [
  translate -0.8 0 0.8
  object cone [
    texture image/boccioni.bmp 2 1
    blend 1.0
    ]
  ]


