import lib.*;

public class Demo {

    static {
        System.loadLibrary("TachyonNativeLib");
    }


    public static void main(String[] args) throws InterruptedException {

        Tachyon tachyon = Tachyon.initialize("indexes.bin");

        String fasta =">d1ux8a_ a.1.1.1 (A:) Protozoan/bacterial hemoglobin {Bacillus subtilis [TaxId: 1423]}\n" +
                "napyeaigeellsqlvdtfyervashpllkpifpsdltetarkqkqfltqylggpplyte\n" +
                "ehghpmlrarhlpfpitneradawlscmkdamdhvglegeireflfgrleltarhmvnq\n" +
                ">d1dlwa_ a.1.1.1 (A:) Protozoan/bacterial hemoglobin {Ciliate (Paramecium caudatum) [TaxId: 5885]}\n" +
                "slfeqlggqaavqavtaqfyaniqadatvatffngidmpnqtnktaaflcaalggpnawt\n" +
                "grnlkevhanmgvsnaqfttvighlrsaltgagvaaalveqtvavaetvrgdvvtv\n";

        TachyonProteineResult result = tachyon.searchP(fasta, 8);

        for (TachyonResultEntry entry : result.getEntries()) {
            String queryName = entry.getQuery().getName();
            queryName = queryName.substring(0, queryName.indexOf(' '));
            for (Alignment alignment : entry.getAlignments()) {
                String targetName = alignment.getTarget().getName();
                targetName = targetName.substring(0, targetName.indexOf(' '));
                System.out.println(queryName + "\t" + targetName + "\t" + alignment.getScore() + "\t" + alignment.getEValue());
            }
        }

        result.delete();
        tachyon.delete();
    }
}
