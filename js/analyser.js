function ANALYSER_SCRIPT(common, $) {
	var PROGRESS_BAR_FAILURE_COLOUR = "#f04124";
	var PROGRESS_BAR_COLOUR = "#008cba";
	var PROGRESS_BAR_SUCCESS_COLOUR = "#43ac6a";

	var GRAPH_MARKING_SIZE = 8;
	var NUMBER_OF_SMALL_MARKINGS_PER_BIG = 10;
	var GUTTER_TEXT_SIZE = 16;

	var colours = {};

	var graph = $("#graphCanvas");
	var modesList = $("#modes");
	var analyserFailureAlertBox = $("#analyserFailureAlert");
	var analyserFailureAlertBoxText = $("#analyserFailureAlert > div");
	var progressBar = $("#uploadProgress");
	var progressBarValue = $("#uploadProgress > .meter");
	var statistics = $("#statistics");
	var statisticsRoot = $("#statisticsRoot");
	var progressBarTargetValue = 0;
	var progressBarTargetColour = PROGRESS_BAR_COLOUR;

	function animateProgressBar() {
		progressBarValue.stop(true);

		progressBarValue.css({
			width: progressBarTargetValue + "%",
			background: progressBarTargetColour
		});
	}

	function hideFail() {
		analyserFailureAlertBox.css({
			"max-height": "0px"
		});
	}

	function fail(html) {
		analyserFailureAlertBox.css({
			"max-height": "1000000em"
		});
		analyserFailureAlertBoxText.html(html);

		setProgressBarStatus(PROGRESS_BAR_FAILURE_COLOUR);
	}

	function hideOutput() {
		graph.addClass("hide");
		statisticsRoot.addClass("hide");
	}

	function setProgressBarStatus(colour) {
		progressBarTargetColour = colour;
		animateProgressBar();
	}

	function setProgressBarValue(value) {
		progressBarTargetValue = value;
		animateProgressBar();
	}

	function MAXIMISING_REDUCER(previousValue, currentValue, index, array) {
		return Math.max(previousValue, currentValue);
	}

	function MINIMISING_REDUCER(previousValue, currentValue, index, array) {
		return Math.min(previousValue, currentValue);
	}

	function SUMMING_REDUCER(previousValue, currentValue, index, array) {
		return previousValue + currentValue;
	}

	function getHumanReadableMonotonicClockTimestamp(nanos) {
		return Math.floor(nanos / 1000000000) + "." + pad((nanos % 1000000000), 9) +
			"s";
	}

	var drawables = {};
	var points = [];
	var accums = [];
	var currentInputDataToValueMapper = 0;
	var mouseInited = false;
	var maximumPointValue;
	var minimumPointValue;

	function pad(num, size) {
		var s = num + "";
		while (s.length < size) s = "0" + s;
		return s;
	}


	Object.filter = function (obj, predicate) {
		var result = {},
			key;

		for (key in obj) {
			if (obj.hasOwnProperty(key) && predicate(obj[key])) {
				result[key] = obj[key];
			}
		}

		return result;
	};

	function TIME_SUFFIX_FORMATTER(val, axis) {
		if (val > 1000000000)
			return (val / 1000000000)
				.toFixed(axis.tickDecimals + 3) + " s";
		return (val / 1000000)
			.toFixed(Math.max(axis.tickDecimals, 1)) + " ms";
	}

	var INPUT_DATA_TO_VALUE_MAPPERS = [{
		name: "&Delta;t: Time between consecutive frames",
		valueName: "&Delta;t",
		suffix: " ms",
		baseDecimals: 3,
		suffixFormatter: TIME_SUFFIX_FORMATTER,
		mapper: function (frames, cur) {
			if (cur === 0)
				return NaN;
			return (frames[cur] - frames[cur - 1]);
		}
	}, {
		name: "f: Frequency (&Delta;t<sup>-1</sup>)",
		valueName: "f",
		baseDecimals: 3,
		suffixFormatter: function suffixFormatter(val, axis) {
			if (val > 1000)
				return (val / 1000)
					.toFixed(axis.tickDecimals + 3) + " kHz";
			return (val)
				.toFixed(axis.tickDecimals) + " Hz";
		},
		mapper: function (frames, cur) {
			if (cur === 0)
				return NaN;
			return 1000000000 / (frames[cur] - frames[cur - 1]);
		}
	}, {
		name: "Frames per second (number of frames in the last second)",
		valueName: "FPS",
		suffix: " FPS",
		baseDecimals: 3,
		suffixFormatter: function suffixFormatter(val, axis) {
			return parseInt(val)
				.toFixed(axis.tickDecimals) + " FPS";
		},
		mapper: function (frames, cur) {
			var i = cur;
			var cnt = 0;
			var hasValue = true;
			while (i >= 0) {
				if (frames[i] + 1000000000 < frames[cur]) {
					hasValue = false;
					break;
				}
				cnt++;
				i--;
			}
			if (hasValue)
				return NaN;
			return cnt;
		}
	}];

	INPUT_DATA_TO_VALUE_MAPPERS.forEach(function (mapper, i) {
		modesList
			.append(
				"<div><input type=\"radio\" name=\"modeRadio\" value=\"+" + i +
				"\" id=\"modeRadio" + i +
				"\"" + ((i == currentInputDataToValueMapper) ? "checked=\"true\"" : "") +
				"><label for=\"modeRadio" + i +
				"\">" + mapper.name + "</label></div>"
		);
		$("#statistics > thead > tr")
			.append("<th>Min " + mapper.name + "</th>")
			.append("<th>Avg " + mapper.name + "</th>")
			.append("<th>Max " + mapper.name + "</th>");
	});
	modesList
		.on("change", function (evt) {
			$('input[name=modeRadio]:checked')
				.each(function () {
					currentInputDataToValueMapper = parseInt($(this)
						.attr("value"));
				});
			renderGraph(true);
		});
	$("#drawables")
		.on("change", function (evt) {
			$.each(drawables, function (key, drawable) {
				drawables[key].show = false;
			});
			$('input[name=drawables]:checked')
				.each(function () {
					drawables[$(this)
						.attr("data-drawableid")].show = true;
				});
			renderGraph(true);
		});

	function calculatePoints(drawable, inputDataToValueMapper) {
		var frames = drawable.data;

		var out = [];
		for (var i = 0; i < frames.length; i++) {
			var data = inputDataToValueMapper.mapper(frames, i);
			if (isNaN(data))
				continue;
			out.push({
				time: frames[i],
				data: data
			});
		}
		return out;
	}

	function calculateAccums(drawable) {
		var frames = drawable.data;
		return INPUT_DATA_TO_VALUE_MAPPERS.map(function (mapper) {
			var curVals = [];
			for (var i = 0; i < frames.length; i++) {
				var data = mapper.mapper(frames, i);
				if (isNaN(data))
					continue;
				curVals.push(data);
			}
			return {
				min: mapper.suffixFormatter(curVals.reduce(MINIMISING_REDUCER, Number.POSITIVE_INFINITY), {
					tickDecimals: mapper.baseDecimals
				}),
				max: mapper.suffixFormatter(curVals.reduce(MAXIMISING_REDUCER, 0), {
					tickDecimals: mapper.baseDecimals
				}),
				average: mapper.suffixFormatter(curVals.reduce(SUMMING_REDUCER, 0) /
					curVals.length, {
						tickDecimals: mapper.baseDecimals
					})
			};
		});
	}

	function reduceAllPoints(points, el, reducer, initial) {
		return Object.keys(points)
			.map(function (key) {
				return points[key].map(function (d) {
						return d[el];
					})
					.reduce(reducer);
			})
			.reduce(reducer, initial);
	}

	function renderGraph(refresh) {
		graph.removeClass("hide");
		statisticsRoot.removeClass("hide");
		if ($("#graphTooltip")
			.length === 0) {
			$("<div id='graphTooltip'></div>")
				.css({
					position: "absolute",
					display: "none",
					border: "1px solid #FFFFFF",
					padding: "0.3333333333em",
					"background-color": "rgb(51, 51, 51)",
					"color": "#FFFFFF",
					opacity: 0.80
				})
				.appendTo("body");
		}

		var currentMapper = INPUT_DATA_TO_VALUE_MAPPERS[currentInputDataToValueMapper];
		if (refresh === true) {
			points = [];
			accums = [];

			Object.keys(drawables)
				.filter(function (key) {
					return drawables[key].show;
				})
				.forEach(function (key) {
					points[key] = calculatePoints(drawables[key], currentMapper);
					accums[key] = calculateAccums(drawables[key]);
				});

			$("#statistics > tbody")
				.html("");
			Object.keys(accums)
				.forEach(function (key) {
					var str = "<tr>" +
						"<td>" + key + "</td>";
					accums[key].forEach(function (reduced, i) {
						str += "<td>" + reduced.min + "</td>";
						str += "<td>" + reduced.average + "</td>";
						str += "<td>" + reduced.max + "</td>";
					});
					str += "</tr>";
					$("#statistics > tbody")
						.append(str);
				});
		}
		var data = Object.keys(points)
			.map(function (key) {
				return {
					label: key,
					data: points[key].map(function (point) {
						return [point.time, point.data];
					})
				};
			});
		var maxY = reduceAllPoints(points, "data", MAXIMISING_REDUCER, 0);
		var minY = reduceAllPoints(points, "data", MINIMISING_REDUCER, 0);
		var maxX = reduceAllPoints(points, "time", MAXIMISING_REDUCER, 0);
		var minX = reduceAllPoints(points, "time", MINIMISING_REDUCER, 0);


		var options = {
			series: {
				lines: {
					show: true
				},
				points: {
					show: false
				}
			},
			crosshair: {
				mode: "xy",
				color: "rgba(60, 60, 60, 0.4)",
				lineWidth: 1
			},
			grid: {
				hoverable: true,
				clickable: true
			},
			xaxis: {
				tickDecimals: 0,
				tickFormatter: TIME_SUFFIX_FORMATTER,
				zoomRange: [minX, maxX],
				panRange: [minX, maxX]
			},
			yaxis: {
				tickDecimals: currentMapper.baseDecimals,
				tickFormatter: currentMapper.suffixFormatter,
				zoomRange: [minY, 5 * maxY / 4],
				panRange: [minY, 5 * maxY / 4]
			},
			zoom: {
				interactive: true,
				trigger: "dblclick",
				amount: 1.5
			},
			pan: {
				interactive: true,
				cursor: "move",
				frameRate: 60
			}
		};

		var plot = $.plot("#graphCanvas", data, options);
		graph.bind("plothover", function (event, pos, item) {
			if (item) {
				var x = item.datapoint[0].toFixed(2),
					y = item.datapoint[1].toFixed(2);
				var tip = $("#graphTooltip");
				tip.html("t: " + TIME_SUFFIX_FORMATTER(x,
					plot.getAxes()
					.xaxis) + "</br>" + currentMapper.valueName + ": " + currentMapper.suffixFormatter(
					y, plot.getAxes()
					.yaxis));
				tip
					.css({
						top: item.pageY,
						left: item.pageX + 10
					})
					.fadeIn(150);
			} else {
				$("#graphTooltip")
					.hide();
			}
		});
		graph.css({
			cursor: "none"
		});
	}

	function parseAndShowFrameLog(frameLog) {
		drawables = {};
		var frames = frameLog.split("\n")
			.map(
				function (line) {
					if (line.length === 0)
						return {};
					var splt = line.split(",");
					var number = /[0-9]+/;
					if (splt.length != 2 || !(number.test(splt[0])) || !(number.test(splt[1]))) {
						throw "Invaild file format! Please check that this is a GLXOSD frame timing log.";
					}
					return {
						drawable: parseInt(splt[0]),
						time: parseInt(splt[1])
					};
				}
			)
			.filter(function (obj) {
				return (Object.keys(obj)
					.length === 2) && (!isNaN(obj.time));
			})
			.sort(
				function (l, r) {
					return l.time - r.time;
				}
			);

		if (frames.length < 5)
			throw "There aren't enough frames in this file for the analysis to be of any use!";

		frames = frames.map(function (frame) {
			return {
				drawable: frame.drawable,
				time: frame.time - frames[0].time
			};
		});

		frames.forEach(function (obj) {
			if (drawables[obj.drawable] === undefined) {
				drawables[obj.drawable] = {
					show: true,
					data: []
				};
			}
			drawables[obj.drawable].data.push(obj.time);
		});

		Object.keys(drawables)
			.forEach(function (drawable) {
				$("#drawables")
					.append("<div><input id=\"drawable" + drawable +
						"\" type=\"checkbox\" name=\"drawables\" data-drawableid=\"" + drawable +
						"\" checked=\"true\"><label for=\"drawable" + drawable + "\">" +
						drawable + "</label></div>");
			});

		renderGraph(true);
	}

	function errorHandler(evt) {
		switch (evt.target.error.code) {
		case evt.target.error.ABORT_ERR:
			break;
		case evt.target.error.NOT_FOUND_ERR:
			fail("File not found!");
			break;
		case evt.target.error.NOT_READABLE_ERR:
			fail("File is not readable!");
			break;
		default:
			fail("Couldn't read file!");
		}
	}

	function handleFileSelect(evt) {
		var files = evt.target.files || (evt.dataTransfer && evt.dataTransfer.files);
		if (files) {
			var file = files[0];
			var output = [];

			var progressBarRoot = progressBar;

			var reader = new FileReader();
			reader.onerror = errorHandler;
			reader.onprogress = function (evt) {
				setProgressBarValue(100 * evt.loaded / evt.total);
			};
			reader.onabort = function (e) {
				setProgressBarValue(0 * evt.loaded / evt.total);
			};
			reader.onloadstart = function (e) {
				hideOutput();
				graph.html("");
				hideFail();
				modesList
					.attr("disabled", "disabled");

				setProgressBarStatus(PROGRESS_BAR_COLOUR);
				setProgressBarValue(0);

				$("#drawables")
					.children()
					.filter("div")
					.remove();
			};
			reader.onload = function (e) {
				try {
					parseAndShowFrameLog(e.target.result);
					setProgressBarStatus(PROGRESS_BAR_SUCCESS_COLOUR);
					setProgressBarValue(100);
					modesList
						.removeAttr("disabled");
				} catch (err) {
					var failMsg = err;
					if (err.stack !== undefined)
						failMsg += "<pre>" + msg.stack + "</pre>";
					fail(failMsg);
				}
			};
			try {
				reader.readAsText(file);
			} catch (msg) {
				fail(msg);
			}
		}
	}

	function handleDragOver(evt) {
		evt.stopPropagation();
		evt.preventDefault();
		evt.originalEvent.dataTransfer.dropEffect = 'copy';
	}
	$(document)
		.ready(function () {
			if (window.File && window.FileReader && window.FileList && window.Blob) {
				var dropZone = $("#dropZone");
				dropZone.on('dragover', handleDragOver);
				dropZone.on('drop', function (evt) {
					evt.stopPropagation();
					evt.preventDefault();
					handleFileSelect(evt.originalEvent);
				});
				$("#logFile")
					.on('change', handleFileSelect);
				$("#analyserRoot")
					.removeClass("hide");
			} else {
				fail("The File APIs are not fully supported in this browser!");
			}
		});
}
require(['common', 'jquery', 'jquery-ui', "jquery.flot",
	"jquery.flot.crosshair",
	"jquery.flot.navigate"
], ANALYSER_SCRIPT);