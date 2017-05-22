/**
 * Created by amowers on 5/21/17.
 */
var mrzSampleId = {
    docType : "P",
    docSubtype : "",
    issuingCountry : "CHE",
    lastName : "Alika Analytics",
    middleName : "Engineering",
    firstName : "Software",
    docNo : "123456789",
    dateBirth: "640126",
    sex : "M",
    dateExpiration: "220618",
    nationality: "CHE",
    personalNumber : "ABC"
};
function mrzGenerate(data) {
    // http://www.highprogrammer.com/cgi-bin/uniqueid/mrzp
    // http://www.emvlab.org/mrz/
    // http://www.highprogrammer.com/alan/numbers/mrp.html#fieldfinalcheck
    var maskLine1 = "Ptiiinnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn"
    var maskLine2 = "NNNNNNNNNCbbbYYMMDDCsyymmddCppppppppppppppCX"

    function repeat(str, times) {
        return (new Array(times + 1)).join(str);
    }
    function pad(str,len) {
        var val = (str || "").toUpperCase();
        return (val + repeat("<",len)).slice(0,len);
    }
    function checkDigit(str) {
        var weights = [7,3,1];
        var r = 0;
        var i;
        for (i = 0; i < str.length; ++i) {
            var n = str.charCodeAt(i);
            if ( (n >= '0'.charCodeAt(0)) && (n <= '9'.charCodeAt(0))) {
                // numbers
                n = n -  '0'.charCodeAt(0);
            } else if (n == '<'.charCodeAt(0)) {
                // < filler
                n = 0;
            } else {
                // letters
                n = n - 'A'.charCodeAt(0) + 10;
            }
            w = weights[i % 3];
            r += n * w

        }
        r %= 10;
        return r;
    }
    function encode(str) {
        return str.toUpperCase().replace(/[^a-z0-9]/gmi, "<");
    }

    var line1 = pad(data.docType,1);
    line1 += pad(data.docSubtype,1);
    line1 += pad(data.issuingCountry,3);
    line1 += pad(
        (data.lastName || "")
        + "<<" + (data.firstName || "")
        +  "<" + (data.middleName || ""),39);
    var docNo = pad(data.docNo,9);
    var line2 = docNo + checkDigit(docNo)
    line2 += pad(data.nationality,3);
    var dateBirth = pad(data.dateBirth,6);
    line2 += dateBirth + checkDigit(dateBirth);
    line2 += pad(data.sex,1);
    var dateExpiration = pad(data.dateExpiration,6);
    line2 += dateExpiration + checkDigit(dateExpiration);
    var personalNumber = pad(data.personalNumber,14);
    line2 += personalNumber + checkDigit(personalNumber);
    var finalCheck = line2.slice(0,10)
        + line2.slice(13,20) + line2.slice(21,43);
    line2 += checkDigit(finalCheck);
    return {
        "line1" : encode(line1),
        "line2" : encode(line2)
    }
}

// Shorthand for $( document ).ready()


