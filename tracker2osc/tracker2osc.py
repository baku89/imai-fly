import triad_openvr
import time
import sys
from pythonosc import osc_message_builder
from pythonosc import udp_client

TRACKER_KEY = "tracker_1"

AXIS_LABELS = ["x", "y", "z"]
class ViveTracker:

	v = None

	client = udp_client.SimpleUDPClient("10.0.1.22", 8000)

	def init(self):
		try:
			self.v = triad_openvr.triad_openvr()
			self.v.print_discovered_objects()
		except:
			print("Error on connecting SteamVR")
			return False

		return True

	def track(self, key, interval):

		if key in self.v.devices == False:
			print("Tracker not found")
			return False

		while(True):
			start = time.time()
			
			try: # avoid halting when zero division occurs
				pose = self.v.devices[key].get_pose_quaternion()
			except Exception as e:
				continue


			# send osc
			x, y, z, qx, qy, qz, qw = pose
			visible = not (x == 0 and y == 0 and z == 0)

			self.client.send_message("/vivetracker/position", [x, y, z])
			self.client.send_message("/vivetracker/quaternion", [qx, qy, qz, qw])
			self.client.send_message("/vivetracker/visible", visible)

			# print
			txt = "Tracking... "
			for i, val in enumerate([x, y, z]):
				txt += AXIS_LABELS[i] + ": " + ("%.2f" % (val * 100)) + " "
			print("\r" + txt, end="")

			# sleep
			sleep_time = interval - (time.time() - start)
			if sleep_time > 0:
				time.sleep(sleep_time)


def main():
	if len(sys.argv) == 1:
		interval = 1/30
	elif len(sys.argv) == 2:
		interval = 1/float(sys.argv[0])
	else:
		print("Invalid number of arguments")
		return

	vt = ViveTracker()

	if vt.init() == False:
		print("Cannot Launch OpenVR")
		return

	vt.track(TRACKER_KEY, interval)


if __name__ == "__main__":
	main()