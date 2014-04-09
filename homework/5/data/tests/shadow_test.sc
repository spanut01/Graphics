camera [
  eye 5 5 5
  up 0 1 0
  focus 0 0 0
  angle 30
  ]

light [
  number 0
  position -2 6 6
  color 0.3 0.0 0.0
  ]

light [
  number 1
  position -1 6 5
  color 0.0 0.3 0.0
  ]

light [
  number 2
  position 0 6 4
  color 0.0 0.0 0.3
  ]

light [
  number 3
  position 1 6 3
  color 0.3 0.3 0.0
  ]

light [
  number 4
  position 2 6 2
  color 0.3 0.0 0.3
  ]

light [
  number 5
  color 0.0 0.3 0.2
  direction 0.5 -0.3 -1
  ]

light [
  number 6
  color 0.3 0.3 0.5
  direction 0.2 -0.2 -1
  ]

light [
  number 7
  color 0.3 0.4 0.3
  direction 0 -0.15 -1
  ]

trans [
  translate 0 -0.6 0
  scale 8 0.1 8
  object cube [
    reflect 1 1 1
    diffuse 0.1 0.1 0.1
    shine 20
    specular 0.5 0.5 0.5
    ]
  ]

trans [
  rotate 1 1 1 60
  object sphere [
    diffuse 0 0 0
    shine 30
    specular 1.0 1.0 1.0
    ]
  ]









