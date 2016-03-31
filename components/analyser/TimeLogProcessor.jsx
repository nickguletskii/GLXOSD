import TimeLogException from "components/analyser/TimeLogException";
import {toNumber as _toNumber, reduce as _reduce, zip as _zip, range as _range, map as _map} from "lodash";
export default class TimeLogProcessor {
	constructor(text) {
		if (text.length === 0) {
			throw new TimeLogException("The uploaded file is empty!");
		}
		const lines = text.split("\n");
		if (lines.length === 0) {
			throw new TimeLogException("The uploaded file is empty!");
		}

		this.lines = lines;

		this.detectLogFormat();
		this.loadData();
		this.generateGraphs();
	}

	generateGraphs() {
		this.graphs = {};
		this.generateDifferencesGraph();
		this.generateFrequenciesGraph();
		this.generateFPSGraph();
	}

	loadData() {
		const nums = [];
		for (let i = 1; i < this.lines.length; i++) {
			if (isNaN(this.lines[i])) {
				throw new TimeLogException("Invalid format!");
			}
			if (this.lines[i].length === 0) {
				continue;
			}
			nums.push(_toNumber(this.lines[i]));
		}

		this.differences = nums;
		// Convert differences into frame times relative to 0.
		this.frames = _reduce(nums, (result, frame) => {
			result.push(result[result.length - 1] + frame);
			return result;
		}, [0]);
	}

	detectLogFormat() {
		const GLXOSD_FORMAT_LINE_REGEX_MATCH = /^# GLXOSD frame log version (\d+.\d+)$/;
		const match = GLXOSD_FORMAT_LINE_REGEX_MATCH.exec(this.lines[0]);
		if (match) {
			this.format = {
				type: "GLXOSD",
				version: match[1]
			};
		} else if (this.lines[0].substr(0, 1) === "#") {
			this.format = {
				type: "voglperf"
			};
		} else {
			throw new TimeLogException("Unknown format. Legacy GLXOSD files are not supported.");
		}
	}

	generateDifferencesGraph() {
		this.graphs.differences = _zip(_range(0, this.differences.length), this.differences);
	}

	generateFrequenciesGraph() {
		this.graphs.frequencies = _zip(_range(0, this.differences.length), _map(this.differences, x => 1000 / x));
	}

	generateFPSGraph() {
		const data = [];
		for (let j = 0; j < this.frames.length; j++) {
			let val = NaN;
			let enoughFrames = false;
			// Walk backwards until we either run out of frames or pass the one second mark
			for (let i = j; i >= 0; i--) {
				if (this.frames[i] + 1000 < this.frames[j]) {
					enoughFrames = true;
					break;
				}
				if (isNaN(val)) {
					val = 1;
				} else {
					val++;
				}
			}
			data.push([
				j, enoughFrames
					? val
					: null
			]);
		}
		this.graphs.fps = data;
	}

}
