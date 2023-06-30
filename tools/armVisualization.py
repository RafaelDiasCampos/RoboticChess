from numpy import pi, sin
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider
import math

dimensions = {
    "waist": 110,
    "shoulder": 120,
    "elbow": 160,
    "wrist": 90,
}

position = {
    "x": 250,
    "y": 0,
    "z": 0
}

def getAngles(x, y, z):
    baseAngle = math.atan2(y, x) * 180 / math.pi

    # Length and height for the shoulder-elbow triangle
    length = math.sqrt(x**2 + y**2)
    height = z - dimensions["waist"] + dimensions["wrist"]
    
    elbowAngleRad = -math.acos((length**2 + height**2 - dimensions["shoulder"]**2 - dimensions["elbow"]**2) / (2 * dimensions["shoulder"] * dimensions["elbow"]))
    elbowAngle = elbowAngleRad * 180 / math.pi

    shoulderAngleRad = math.atan2(height, length) - math.atan2(dimensions["elbow"] * math.sin(elbowAngleRad), dimensions["shoulder"] + dimensions["elbow"] * math.cos(elbowAngleRad))
    shoulderAngle = shoulderAngleRad * 180 / math.pi

    wristAngle = -90 - elbowAngle - shoulderAngle

    return (baseAngle, shoulderAngle, elbowAngle, wristAngle)

def plotArm(position, plots):
    [baseAngle, shoulderAngle, elbowAngle, wristAngle] = getAngles(position["x"], position["y"], position["z"])

    print(f"Base: {baseAngle}, Shoulder: {shoulderAngle}, Elbow: {elbowAngle}, Wrist: {wristAngle}")

    # Plot the waist
    plots["waist"].set_data_3d([0, 0], [0, 0], [0, dimensions["waist"]])

    # Plot the shoulder
    shoulder_xy_length = dimensions["shoulder"] * math.cos(shoulderAngle * math.pi / 180)
    shoulder_z_end = dimensions["shoulder"] * math.sin(shoulderAngle * math.pi / 180) + dimensions["waist"]
    shoulder_x_end = shoulder_xy_length * math.cos(baseAngle * math.pi / 180)
    shoulder_y_end = shoulder_xy_length * math.sin(baseAngle * math.pi / 180)
    
    plots["shoulder"].set_data_3d([0, shoulder_x_end], [0, shoulder_y_end], [dimensions["waist"], shoulder_z_end])

    # Plot the elbow
    elbow_xy_length = dimensions["elbow"] * math.cos((elbowAngle + shoulderAngle) * math.pi / 180)
    elbow_z_end = dimensions["elbow"] * math.sin((elbowAngle + shoulderAngle) * math.pi / 180) + shoulder_z_end
    elbow_x_end = elbow_xy_length * math.cos((baseAngle) * math.pi / 180) + shoulder_x_end
    elbow_y_end = elbow_xy_length * math.sin((baseAngle) * math.pi / 180) + shoulder_y_end

    plots["elbow"].set_data_3d([shoulder_x_end, elbow_x_end], [shoulder_y_end, elbow_y_end], [shoulder_z_end, elbow_z_end])

    # Plot the wrist
    wrist_xy_length = dimensions["wrist"] * math.cos((wristAngle + elbowAngle + shoulderAngle) * math.pi / 180)
    wrist_z_end = dimensions["wrist"] * math.sin((wristAngle + elbowAngle + shoulderAngle) * math.pi / 180) + elbow_z_end
    wrist_x_end = wrist_xy_length * math.cos((baseAngle) * math.pi / 180) + elbow_x_end
    wrist_y_end = wrist_xy_length * math.sin((baseAngle) * math.pi / 180) + elbow_y_end

    plots["wrist"].set_data_3d([elbow_x_end, wrist_x_end], [elbow_y_end, wrist_y_end], [elbow_z_end, wrist_z_end])


axis_color = 'lightgoldenrodyellow'

# Create a figure and a 3d plot
fig = plt.figure()
ax = fig.add_subplot(projection='3d')

# Adjust the subplots region to leave some space for the sliders
fig.subplots_adjust(bottom=0.25, top=1)

# Draw the initial plot
# The 'line' variable is used for modifying the line later
plots = {
    "waist": ax.plot([0, 0], [0, 0], [0, 0], linewidth=4, color='black')[0],
    "shoulder": ax.plot([0, 0], [0, 0], [0, 0], linewidth=4, color='blue')[0],
    "elbow": ax.plot([0, 0], [0, 0], [0, 0], linewidth=4, color='green')[0],
    "wrist": ax.plot([0, 0], [0, 0], [0, 0], linewidth=4, color='red')[0],
}
plotArm(position, plots)

ax.set_xlim([-300, 300])
ax.set_ylim([-300, 300])
ax.set_zlim([0, 300])

# Add two sliders for tweaking the parameters

# Define an axes area and draw a slider in it
x_slider_ax  = fig.add_axes([0.25, 0.15, 0.65, 0.03], facecolor=axis_color)
x_slider = Slider(x_slider_ax, 'X', -300, 300, valinit=position["x"])

# Draw another slider
y_slider_ax = fig.add_axes([0.25, 0.1, 0.65, 0.03], facecolor=axis_color)
y_slider = Slider(y_slider_ax, 'Y', -300, 300, valinit=position["y"])

# Draw another slider
z_slider_ax = fig.add_axes([0.25, 0.05, 0.65, 0.03], facecolor=axis_color)
z_slider = Slider(z_slider_ax, 'Z', -300, 300, valinit=position["z"])

# Define an action for modifying the line when any slider's value changes
def sliders_on_changed(val):
    position["x"] = x_slider.val
    position["y"] = y_slider.val
    position["z"] = z_slider.val
    plotArm(position, plots)
    fig.canvas.draw_idle()
    
x_slider.on_changed(sliders_on_changed)
y_slider.on_changed(sliders_on_changed)
z_slider.on_changed(sliders_on_changed)

plt.show()

pause = input("Press enter to continue...")