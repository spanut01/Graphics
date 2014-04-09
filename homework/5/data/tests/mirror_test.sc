camera [
  eye -6 4 4
  focus 0 0 0
  up 0 1 0
  angle 30
  ]

light [
  number 0
  color 0.8 0.8 0.8
  direction 0 -1 -1
  ]

light [
  number 1
  color 0.4 0.4 0.4
  direction 1 -1 0
  ]

trans [
  translate -0.65 0 0.65
  object sphere [
    shine 15.0
    specular 1.0 1.0 1.0
    reflect 0.8 0.6 0.6
    diffuse 0.8 0.6 0.6
    ]
  ]

trans [
  translate -0.65 0 -0.65
  object sphere [
    shine 15.0
    specular 1.0 1.0 1.0
    reflect 0.6 0.6 0.8
    diffuse 0.6 0.6 0.8
    ]
  ]

trans [
  translate 0.65 0 -0.65
  object sphere [
    shine 15.0
    specular 1.0 1.0 1.0
    reflect 0.6 0.8 0.6
    diffuse 0.6 0.8 0.6
    ]
  ]


trans [
  translate 0.65 0 0.65
  object sphere [
    shine 15.0
    specular 1.0 1.0 1.0
    reflect 0.6 0.6 0.6
    diffuse 0.7 0.7 0.7
    ]
  ]

trans [
  translate 0 -0.6 0
  scale 6 0.1 6
  object cube [
    diffuse 0 1 1
    texture image/ballz5.bmp 1 1
    reflect 0.3 0.3 0.3
    blend 0.9
    ]
  ]



